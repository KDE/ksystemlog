#include <QRegularExpression>

#include <libaudit.h>

#include <fcntl.h>
#include <grp.h>
#include <linux/prctl.h>
#include <netdb.h>
#include <pwd.h>
#include <sys/personality.h>
#include <sys/stat.h>

#include "maps.h"

#include "auditConvertor.h"

AuditConvertor::AuditConvertor()
    : m_a0(-1)
    , m_a1(-1)
{
    m_paramMap = {{QStringLiteral("arch"), &AuditConvertor::convertArch},
                  {QStringLiteral("exit"), &AuditConvertor::convertExit},
                  {QStringLiteral("uid"), &AuditConvertor::convertUid},
                  {QStringLiteral("auid"), &AuditConvertor::convertUid},
                  {QStringLiteral("old-auid"), &AuditConvertor::convertUid},
                  {QStringLiteral("euid"), &AuditConvertor::convertUid},
                  {QStringLiteral("suid"), &AuditConvertor::convertUid},
                  {QStringLiteral("fsuid"), &AuditConvertor::convertUid},
                  {QStringLiteral("ouid"), &AuditConvertor::convertUid},
                  {QStringLiteral("gid"), &AuditConvertor::convertGid},
                  {QStringLiteral("egid"), &AuditConvertor::convertGid},
                  {QStringLiteral("sgid"), &AuditConvertor::convertGid},
                  {QStringLiteral("fsgid"), &AuditConvertor::convertGid},
                  {QStringLiteral("ogid"), &AuditConvertor::convertGid},
                  {QStringLiteral("mode"), &AuditConvertor::convertMode},
                  {QStringLiteral("a2"), &AuditConvertor::convertA2},
                  {QStringLiteral("a3"), &AuditConvertor::convertA3}};
}

QString AuditConvertor::getParam(const QString &message, const QString &name)
{
    const QRegularExpression re(QStringLiteral("\\s%1=([^\\s]*)").arg(name));
    const QRegularExpressionMatch match = re.match(message);

    if (match.hasMatch()) {
        return match.captured(1);
    } else {
        return QString();
    }
}

QString AuditConvertor::replaceParam(const QString &message, const QString &name, const QString &value)
{
    const QRegularExpression re(QStringLiteral("\\s%1=[^\\s]*").arg(name));
    QString newMessage = message;

    return newMessage.replace(re, QStringLiteral(" %1=%2").arg(name).arg(value));
}

QString AuditConvertor::convertMessage(const QString &message)
{
    QString newMessage = message;

    const QString proctitleParam = QStringLiteral("proctitle");
    QString proctitle = getParam(message, proctitleParam);

    if (!proctitle.isEmpty()) {
        proctitle = convertProcTitle(proctitle);
        return replaceParam(newMessage, proctitleParam, proctitle);
    }

    const QString syscallParam = QStringLiteral("syscall");
    QString syscall = getParam(message, syscallParam);

    if (!syscall.isEmpty()) {
        syscall = convertSysCall(syscall);
        newMessage = replaceParam(newMessage, syscallParam, syscall);
        m_syscall = syscall;
    } else {
        m_syscall.clear();
    }

    m_a0 = -1;
    m_a1 = -1;

    bool ok = false;

    if (!m_syscall.isEmpty()) {
        const QString a0Param = QStringLiteral("a0");
        QString a0 = getParam(message, a0Param);

        if (!a0.isEmpty()) {
            m_a0 = a0.toULong(&ok, 16);
            a0 = convertA0(a0);
            newMessage = replaceParam(newMessage, a0Param, a0);
        }

        const QString a1Param = QStringLiteral("a1");
        QString a1 = getParam(message, a1Param);

        if (!a1.isEmpty()) {
            m_a1 = a1.toULong(&ok, 16);
            a1 = convertA1(a1);
            newMessage = replaceParam(newMessage, a1Param, a1);
        }
    }

    convertParamFunc func = nullptr;
    QString name;
    QString value;

    auto it = m_paramMap.constBegin();

    while (it != m_paramMap.constEnd()) {
        func = it.value();
        name = it.key();
        value = getParam(message, name);

        if (!value.isEmpty()) {
            value = (this->*func)(value);
            newMessage = replaceParam(newMessage, name, value);
        }

        ++it;
    }

    return newMessage;
}

QString AuditConvertor::convertProcTitle(const QString &proctitle)
{
    QString newProcTitle = proctitle;

    if (proctitle[0] == QChar::fromLatin1('\"')) {
        return newProcTitle.remove(QChar::fromLatin1('\"'));
    }

    newProcTitle.replace(QLatin1String("00"), QLatin1String("20"));

    QByteArray arr;
    QString str;
    bool ok;

    for (int i = 0; i < proctitle.size(); i += 2) {
        str = newProcTitle.mid(i, 2);
        arr.append(char(str.toShort(&ok, 16)));
    }

    return QString::fromLatin1(arr);
}

QString AuditConvertor::convertArch(const QString &arch)
{
    bool ok = false;

    unsigned int machine = arch.toUInt(&ok, 16);
    machine = audit_elf_to_machine(machine);

    const char *name = audit_machine_to_name(machine);

    return QString::fromLatin1(name);
}

QString AuditConvertor::convertSysCall(const QString &syscall)
{
    int machine = audit_detect_machine();
    const char *name = audit_syscall_to_name(syscall.toInt(), machine);

    return QString::fromLatin1(name);
}

QString AuditConvertor::convertExit(const QString &exit)
{
    long long ival = exit.toLongLong();

    if (ival < 0) {
        const char *name = audit_errno_to_name(-ival);
        return QStringLiteral("%1(%2)").arg(QString::fromLocal8Bit(name)).arg(QString::fromLocal8Bit(strerror(-ival)));
    } else {
        return exit;
    }
}

QString AuditConvertor::convertUid(const QString &uid, int base)
{
    bool ok = false;
    int val = uid.toInt(&ok, base);

    if (val == -1) {
        return QStringLiteral("unset");
    } else if (val == 0) {
        return QStringLiteral("root");
    }

    struct passwd *pw = getpwuid(val);

    if (pw) {
        return QString::fromLatin1(pw->pw_name);
    } else {
        return QStringLiteral("unknown(%1)").arg(uid);
    }
}

QString AuditConvertor::convertUid(const QString &uid)
{
    return convertUid(uid, 10);
}

QString AuditConvertor::convertGid(const QString &gid, int base)
{
    bool ok = false;
    int val = gid.toInt(&ok, base);

    if (val == -1) {
        return QStringLiteral("unset");
    } else if (val == 0) {
        return QStringLiteral("root");
    }

    struct group *gr = getgrgid(val);

    if (gr) {
        return QString::fromLatin1(gr->gr_name);
    } else {
        return QStringLiteral("unknown(%1)").arg(gid);
    }
}

QString AuditConvertor::convertGid(const QString &gid)
{
    return convertGid(gid, 10);
}

QString AuditConvertor::convertMode(const QString &mode, int base)
{
    bool ok = false;
    unsigned int ival = mode.toULong(&ok, base);
    const char *name;
    QString newMode;

    name = audit_ftype_to_name(ival & S_IFMT);

    if (name) {
        newMode = QString::fromLatin1(name);
    } else {
        unsigned first_ifmt_bit = S_IFMT & ~(S_IFMT - 1);
        newMode = QStringLiteral("0%1").arg((ival & S_IFMT) / first_ifmt_bit, 3, 8, QLatin1Char('0'));
    }

    if (S_ISUID & ival) {
        newMode += QLatin1String(",suid");
    }
    if (S_ISGID & ival) {
        newMode += QLatin1String(",sgid");
    }
    if (S_ISVTX & ival) {
        newMode += QLatin1String(",sticky");
    }

    newMode += QStringLiteral(",0%1").arg((S_IRWXU | S_IRWXG | S_IRWXO) & ival, 3, 8, QLatin1Char('0'));

    return newMode;
}

QString AuditConvertor::convertMode(const QString &mode)
{
    return convertMode(mode, 8);
}

QString AuditConvertor::convertA0(const QString &a0)
{
    if (m_syscall == QLatin1String("rt_sigaction")) {
        return convertSignals(a0, 16);
    } else if ((m_syscall == QLatin1String("renameat")) || (m_syscall == QLatin1String("readlinkat")) || (m_syscall == QLatin1String("mkdirat"))
               || (m_syscall == QLatin1String("mknodat")) || (m_syscall == QLatin1String("fchownat")) || (m_syscall == QLatin1String("futimesat"))
               || (m_syscall == QLatin1String("fchmodat")) || (m_syscall == QLatin1String("faccessat")) || (m_syscall == QLatin1String("futimensat"))
               || (m_syscall == QLatin1String("unlinkat")) || (m_syscall == QLatin1String("utimensat")) || (m_syscall == QLatin1String("linkat"))
               || (m_syscall == QLatin1String("newfstatat")) || (m_syscall == QLatin1String("openat"))) {
        return convertDirFd(a0);
    } else if ((m_syscall == QLatin1String("clone")) || (m_syscall == QLatin1String("unshare"))) {
        return convertCloneFlags(a0);
    } else if (m_syscall == QLatin1String("clock_settime")) {
        return convertClockId(a0);
    } else if (m_syscall == QLatin1String("personality")) {
        return convertPersonality(a0);
    } else if (m_syscall == QLatin1String("ptrace")) {
        return convertPtrace(a0);
    } else if (m_syscall == QLatin1String("prctl")) {
        return convertPrctlOpt(a0);
    } else if ((m_syscall == QLatin1String("getrlimit")) || (m_syscall == QLatin1String("setrlimit"))) {
        return convertRlimit(a0);
    } else if ((m_syscall == QLatin1String("setuid")) || (m_syscall == QLatin1String("setreuid")) || (m_syscall == QLatin1String("setresuid"))
               || (m_syscall == QLatin1String("setfsuid"))) {
        return convertUid(a0, 16);
    } else if ((m_syscall == QLatin1String("setgid")) || (m_syscall == QLatin1String("setregid")) || (m_syscall == QLatin1String("setresgid"))
               || (m_syscall == QLatin1String("setfsgid"))) {
        return convertGid(a0, 16);
    } else if (m_syscall == QLatin1String("socket")) {
        return convertSocketDomain(a0);
    } else if (m_syscall == QLatin1String("socketcall")) {
        return convertSocketCall(a0, 16);
    } else if (m_syscall == QLatin1String("ipccall")) {
        return convertIpcCall(a0, 16);
    } else if ((m_syscall == QLatin1String("exit")) || (m_syscall == QLatin1String("exit_group"))) {
        return convertExitSysCall(a0);
    } else if (m_syscall == QLatin1String("bpf")) {
        return convertBpf(a0);
    } else {
        return QStringLiteral("0x") + a0;
    }
}

QString AuditConvertor::convertA1(const QString &a1)
{
    if ((m_syscall == QLatin1String("fchmod")) || (m_syscall == QLatin1String("chmod")) || (m_syscall == QLatin1String("creat"))
        || (m_syscall == QLatin1String("mkdir"))) {
        return convertModeShort(a1, 16);
    } else if (m_syscall.indexOf(QLatin1String("fcntl")) == 0) {
        return convertFcntlCmd(a1);
    } else if ((m_syscall == QLatin1String("chown")) || (m_syscall == QLatin1String("setreuid")) || (m_syscall == QLatin1String("setresuid"))) {
        return convertUid(a1, 16);
    } else if (m_syscall == QLatin1String("etsockopt")) {
        return convertSockOptLevel(a1);
    } else if ((m_syscall == QLatin1String("setregid")) || (m_syscall == QLatin1String("setresgid"))) {
        return convertGid(a1, 16);
    } else if (m_syscall == QLatin1String("socket")) {
        return convertSocketType(a1);
    } else if (m_syscall == QLatin1String("setns")) {
        return convertCloneFlags(a1);
    } else if (m_syscall == QLatin1String("sched_setscheduler")) {
        return convertSched(a1);
    } else if (m_syscall == QLatin1String("mknod")) {
        return convertMode(a1, 16);
    } else if ((m_syscall == QLatin1String("mq_open")) || (m_syscall == QLatin1String("open"))) {
        return convertOpenFlags(a1);
    } else if (m_syscall == QLatin1String("access")) {
        return convertAccess(a1);
    } else if (m_syscall == QLatin1String("epoll_ctl")) {
        return convertEpollCtl(a1);
    } else if ((m_syscall == QLatin1String("kill")) || (m_syscall == QLatin1String("tkill"))) {
        return convertSignals(a1, 16);
    } else if (m_syscall == QLatin1String("prctl")) {
        if (m_a0 == PR_CAPBSET_READ || m_a0 == PR_CAPBSET_DROP) {
            return convertCapabilities(a1, 16);
        } else if (m_a0 == PR_SET_PDEATHSIG) {
            return convertSignals(a1, 16);
        }
    } else if (m_syscall == QLatin1String("umount2")) {
        return convertUmount(a1);
    } else if (m_syscall == QLatin1String("ioctl")) {
        return convertIoctlReq(a1);
    }

    return QStringLiteral("0x") + a1;
}

QString AuditConvertor::convertA2(const QString &a2)
{
    if (m_syscall.indexOf(QLatin1String("fcntl")) == 0) {
        return convertFcntl(a2);
    } else if (m_syscall.indexOf(QLatin1String("etsockopt")) == 1) {
        if (m_a1 == IPPROTO_IP) {
            return convertIpOptName(a2);
        } else if (m_a1 == SOL_SOCKET) {
            return convertSockOptName(a2);
        } else if (m_a1 == IPPROTO_TCP) {
            return convertTcpOptName(a2);
        } else if (m_a1 == IPPROTO_UDP) {
            return convertUdpOptName(a2);
        } else if (m_a1 == IPPROTO_IPV6) {
            return convertIp6OptName(a2);
        } else if (m_a1 == SOL_PACKET) {
            return convertPktOptName(a2);
        }
    } else if (m_syscall == QLatin1String("openat")) {
        return convertOpenFlags(a2);
    } else if ((m_syscall == QLatin1String("open")) && (m_a1 & O_CREAT)) {
        return convertModeShort(a2, 16);
    } else if ((m_syscall == QLatin1String("fchmodat")) || (m_syscall == QLatin1String("mkdirat")) || (m_syscall == QLatin1String("mknodat"))) {
        return convertModeShort(a2, 16);
    } else if (m_syscall == QLatin1String("faccessat")) {
        return convertAccess(a2);
    } else if (m_syscall == QLatin1String("setresuid")) {
        return convertUid(a2, 16);
    } else if ((m_syscall == QLatin1String("setresgid")) || (m_syscall == QLatin1String("chown"))) {
        return convertGid(a2, 16);
    } else if (m_syscall == QLatin1String("socket")) {
        return convertSocketProto(a2);
    } else if ((m_syscall == QLatin1String("sendmsg")) || (m_syscall == QLatin1String("recvmsg"))) {
        return convertRecv(a2);
    } else if (m_syscall == QLatin1String("shmget")) {
        return convertShmFlags(a2);
    } else if (m_syscall == QLatin1String("mmap")) {
        return convertProt(a2, 1);
    } else if (m_syscall == QLatin1String("mprotect")) {
        return convertProt(a2, 0);
    } else if (m_syscall == QLatin1String("mq_open") && (m_a1 & O_CREAT)) {
        return convertModeShort(a2, 16);
    } else if ((m_syscall == QLatin1String("readlinkat")) || (m_syscall == QLatin1String("linkat"))) {
        return convertDirFd(a2);
    } else if (m_syscall == QLatin1String("lseek")) {
        return convertSeek(a2);
    } else if (m_syscall == QLatin1String("tgkill")) {
        return convertSignals(a2, 16);
    }

    return QStringLiteral("0x") + a2;
}

QString AuditConvertor::convertA3(const QString &a3)
{
    if (m_syscall == QLatin1String("mmap")) {
        return convertMmap(a3);
    } else if (m_syscall == QLatin1String("mount")) {
        return convertMount(a3);
    } else if ((m_syscall == QLatin1String("recv")) || (m_syscall == QLatin1String("recvfrom")) || (m_syscall == QLatin1String("recvmmsg"))
               || (m_syscall == QLatin1String("send")) || (m_syscall == QLatin1String("sendto")) || (m_syscall == QLatin1String("sendmmsg"))) {
        return convertRecv(a3);
    }

    return QStringLiteral("0x") + a3;
}

QString AuditConvertor::convertSignals(const QString &sig, unsigned int base)
{
    bool ok = false;
    ulong ival = sig.toULong(&ok, base);
    QString value;

    if (!ok) {
        return convertError(sig);
    } else if (signalMap.contains(ival)) {
        return signalMap.value(ival);
    } else {
        if (base == 16) {
            value = QLatin1String("0x");
        }
        value += sig;
        return QStringLiteral("unknown-signal(%1)").arg(value);
    }
}

QString AuditConvertor::convertDirFd(const QString &dirfd)
{
    if (dirfd == QLatin1String("-100")) {
        return QStringLiteral("AT_FDCWD");
    } else {
        return QStringLiteral("0x") + dirfd;
    }
}

QString AuditConvertor::convertCloneFlags(const QString &cloneFlags)
{
    ulong clone_sig;
    int cnt = 0;
    QString retValue;

    bool ok = false;
    ulong flags;

    flags = cloneFlags.toULong(&ok, 16);

    if (!ok) {
        return convertError(cloneFlags);
    }

    auto it = cloneFlagMap.constBegin();
    while (it != cloneFlagMap.constEnd()) {
        if (it.key() & flags) {
            if (cnt) {
                retValue += QLatin1String("|");
            }
            retValue += it.value();
            cnt++;
        }
        ++it;
    }

    clone_sig = flags & 0xFF;

    if (signalMap.contains(clone_sig)) {
        if (cnt) {
            retValue += QLatin1String("|");
        }
        retValue += signalMap.value(clone_sig);
    }

    if (retValue.isEmpty()) {
        return QStringLiteral("0x%1").arg(cloneFlags);
    } else {
        return retValue;
    }
}

QString AuditConvertor::convertClockId(const QString &clockId)
{
    bool ok = false;
    ulong ival = clockId.toULong(&ok, 16);

    if (!ok) {
        return convertError(clockId);
    } else {
        return clockMap.value(ival, QStringLiteral("unknown-clk_id(0x%1)").arg(clockId));
    }
}

QString AuditConvertor::convertPersonality(const QString &personality)
{
    bool ok = false;
    ulong pers, pers2;

    pers = personality.toULong(&ok, 16);

    if (!ok) {
        return convertError(personality);
    }

    pers2 = pers & PER_MASK;

    if (personMap.contains(pers2)) {
        if (pers & ADDR_NO_RANDOMIZE) {
            return QStringLiteral("%1|~ADDR_NO_RANDOMIZE").arg(personMap.value(pers2));
        } else {
            return personMap.value(pers2);
        }
    } else {
        return QStringLiteral("unknown-personality(0x%1)").arg(personality);
    }
}

QString AuditConvertor::convertPtrace(const QString &ptrace)
{
    bool ok = false;
    int trace = ptrace.toInt(&ok, 16);

    if (!ok) {
        return convertError(ptrace);
    } else {
        return ptraceMap.value(trace, QStringLiteral("unknown-ptrace(0x%1)").arg(ptrace));
    }
}

QString AuditConvertor::convertPrctlOpt(const QString &prctlOpt)
{
    bool ok = false;
    int opt = prctlOpt.toUInt(&ok, 16);

    if (!ok) {
        return convertError(prctlOpt);
    } else {
        return prctloptMap.value(opt, QStringLiteral("unknown-prctl-option(0x%1)").arg(prctlOpt));
    }
}

QString AuditConvertor::convertRlimit(const QString &rlimit)
{
    bool ok = false;
    ulong ival = rlimit.toULong(&ok, 16);

    if (!ok) {
        return convertError(rlimit);
    } else {
        return rlimitMap.value(ival, QStringLiteral("unknown-rlimit(0x%1)").arg(rlimit));
    }
}

QString AuditConvertor::convertSocketDomain(const QString &socketDomain)
{
    bool ok = false;
    int ival = socketDomain.toULong(&ok, 16);

    if (!ok) {
        return convertError(socketDomain);
    } else {
        return famMap.value(ival, QStringLiteral("unknown-family(0x%1)").arg(socketDomain));
    }
}

QString AuditConvertor::convertSocketCall(const QString &socketCall, unsigned int base)
{
    bool ok = false;
    int a0 = socketCall.toLong(&ok, base);

    if (!ok) {
        return convertError(socketCall);
    } else {
        return sockMap.value(a0, QStringLiteral("unknown-socketcall(%1)").arg(socketCall));
    }
}

QString AuditConvertor::convertIpcCall(const QString &ipcCall, unsigned int base)
{
    bool ok = false;
    int a0 = ipcCall.toLong(&ok, base);

    if (!ok) {
        return convertError(ipcCall);
    } else {
        return ipcMap.value(a0, QStringLiteral("unknown-ipccall(%1)").arg(ipcCall));
    }
}

QString AuditConvertor::convertExitSysCall(const QString &exitSysCall)
{
    if (exitSysCall == QLatin1String("0")) {
        return QStringLiteral("EXIT_SUCCESS");
    } else if (exitSysCall == QLatin1String("1")) {
        return QStringLiteral("EXIT_FAILURE");
    } else {
        return QStringLiteral("UNKNOWN");
    }
}

QString AuditConvertor::convertBpf(const QString &bpf)
{
    bool ok = false;
    ulong cmd = bpf.toULong(&ok, 16);

    if (!ok) {
        return convertError(bpf);
    } else {
        return bpfMap.value(cmd, QStringLiteral("unknown-bpf-cmd(%1)").arg(bpf));
    }
}

QString AuditConvertor::convertModeShort(const QString &mode, int base)
{
    bool ok = false;
    int val = mode.toULong(&ok, base);

    if (!ok) {
        return convertError(mode);
    } else {
        return convertModeShortInt(val);
    }
}

QString AuditConvertor::convertModeShortInt(int mode)
{
    QString newMode;

    if (S_ISUID & mode) {
        newMode = QLatin1String("suid");
    }

    if (S_ISGID & mode) {
        if (!newMode.isEmpty()) {
            newMode += QLatin1String(",");
        }
        newMode += QLatin1String("sgid");
    }
    if (S_ISVTX & mode) {
        if (!newMode.isEmpty()) {
            newMode += QLatin1String(",");
        }
        newMode += QLatin1String("sticky");
    }

    if (!newMode.isEmpty()) {
        newMode += QStringLiteral(",");
    }

    return newMode += QStringLiteral("0%1").arg((S_IRWXU | S_IRWXG | S_IRWXO) & mode, 3, 8, QLatin1Char('0'));
}

QString AuditConvertor::convertFcntlCmd(const QString &cmd)
{
    bool ok = false;
    int ival = cmd.toULong(&ok, 16);

    if (!ok) {
        return convertError(cmd);
    } else {
        return fcntlcmdMap.value(ival, QStringLiteral("unknown-fcntl-command(%1)").arg(ival));
    }
}

QString AuditConvertor::convertSockOptLevel(const QString &level)
{
    bool ok = false;
    int ival = level.toULong(&ok, 16);

    if (!ok) {
        return convertError(level);
    } else if (ival == SOL_SOCKET) {
        return QStringLiteral("SOL_SOCKET");
    } else {
        protoent *p = getprotobynumber(ival);
        if (!p) {
            return socklevelMap.value(ival, QStringLiteral("unknown-sockopt-level(0x%1)").arg(level));
        } else {
            return QString::fromLatin1(p->p_name);
        }
    }
}

QString AuditConvertor::convertSocketType(const QString &type)
{
    bool ok = false;
    ulong ival = type.toULong(&ok, 16);

    if (!ok) {
        return convertError(type);
    } else {
        return sockMap.value(ival, QStringLiteral("unknown-type(%1)").arg(type));
    }
}

QString AuditConvertor::convertSched(const QString &sched)
{
    bool ok = false;
    ulong pol = sched.toULong(&ok, 16);

    if (!ok) {
        return convertError(sched);
    }

    QString retValue = schedMap.value(pol & 0x0F, QStringLiteral("unknown-scheduler-policy(0x%1)").arg(sched));

    if (schedMap.contains(pol & 0x0F) && (pol & 0x40000000)) {
        retValue += QLatin1String("|SCHED_RESET_ON_FORK");
    }

    return retValue;
}

QString AuditConvertor::convertOpenFlags(const QString &flags)
{
    bool ok = false;
    ulong ival = flags.toULong(&ok, 16);
    int cnt = 0;
    QString retValue;

    if (!ok) {
        return convertError(flags);
    }

    if ((ival & O_ACCMODE) == 0) {
        retValue = QLatin1String("O_RDONLY");
        cnt++;
    }

    auto it = openflagMap.constBegin();
    while (it != openflagMap.constEnd()) {
        if (it.key() & ival) {
            if (cnt) {
                retValue += QLatin1String("|");
            }

            retValue += it.value();
            cnt++;
        }
        ++it;
    }

    if (retValue.isEmpty()) {
        return QStringLiteral("0x%1").arg(flags);
    } else {
        return retValue;
    }
}

QString AuditConvertor::convertAccess(const QString &access)
{
    bool ok = false;
    ulong mode = access.toULong(&ok, 16);
    QString retValue;
    unsigned int cnt = 0;

    if (!ok) {
        return convertError(access);
    }

    if ((mode & 0xF) == 0) {
        return QString::fromLatin1("F_OK");
    }

    auto it = accessMap.constBegin();
    while (it != accessMap.constEnd()) {
        if (it.key() & mode) {
            if (cnt) {
                retValue += QLatin1String("|");
            }
            retValue += it.value();
            cnt++;
        }
        ++it;
    }

    if (retValue.isEmpty()) {
        return QStringLiteral("0x%1").arg(access);
    } else {
        return retValue;
    }
}

QString AuditConvertor::convertEpollCtl(const QString &ctl)
{
    bool ok = false;
    ulong cmd = ctl.toULong(&ok, 16);

    if (!ok) {
        return convertError(ctl);
    } else {
        return epollctlMap.value(cmd, QStringLiteral("unknown-epoll_ctl-operation(%1)").arg(cmd));
    }
}

QString AuditConvertor::convertCapabilities(const QString &capabilities, int base)
{
    bool ok = false;
    ulong cap = capabilities.toULong(&ok, base);
    QString value;

    if (!ok) {
        return convertError(capabilities);
    } else if (capMap.contains(cap)) {
        return capMap.value(cap);
    } else {
        if (base == 16) {
            value = QLatin1String("0x");
        }
        value += capabilities;
        return QStringLiteral("unknown-capability(%1)").arg(value);
    }
}

QString AuditConvertor::convertUmount(const QString &umount)
{
    bool ok = false;
    ulong flags = umount.toULong(&ok, 16);
    int cnt = 0;
    QString retValue;

    if (!ok) {
        return convertError(umount);
    }

    auto it = umountMap.constBegin();
    while (it != umountMap.constEnd()) {
        if (it.key() & flags) {
            if (cnt) {
                retValue += QLatin1String("|");
            }

            retValue += it.value();
            cnt++;
        }
        ++it;
    }

    if (retValue.isEmpty()) {
        return QStringLiteral("0x%1").arg(umount);
    } else {
        return retValue;
    }
}

QString AuditConvertor::convertIoctlReq(const QString &ioctlReq)
{
    bool ok = false;
    ulong req = ioctlReq.toULong(&ok, 16);

    if (!ok) {
        return convertError(ioctlReq);
    } else {
        return ioctlreqMap.value(req, QStringLiteral("0x%1").arg(ioctlReq));
    }
}

QString AuditConvertor::convertFcntl(const QString &fcntl)
{
    bool ok = false;
    int val = fcntl.toULong(&ok, 16);

    if (!ok) {
        return convertError(fcntl);
    }

    switch (m_a1) {
    case F_SETOWN:
        return convertUid(fcntl, 16);
    case F_SETFD:
        if (val == FD_CLOEXEC) {
            return QString::fromLatin1("FD_CLOEXEC");
        } else {
            return QString();
        }
    default:
        return fcntl;
    }
}

QString AuditConvertor::convertIpOptName(const QString &name)
{
    bool ok = false;
    ulong opt = name.toULong(&ok, 16);

    if (!ok) {
        return convertError(name);
    } else {
        return ipoptnameMap.value(opt, QStringLiteral("unknown-ipopt-name(0x%1)").arg(name));
    }
}

QString AuditConvertor::convertSockOptName(const QString &name)
{
    bool ok = false;
    ulong opt = name.toULong(&ok, 16);

    if (!ok) {
        return convertError(name);
    }

    int machine = audit_detect_machine();

    if ((machine == MACH_PPC64 || machine == MACH_PPC) && opt >= 16 && opt <= 21) {
        opt += 100;
    }

    return sockoptnameMap.value(opt, QStringLiteral("unknown-sockopt-name(0x%1)").arg(name));
}

QString AuditConvertor::convertTcpOptName(const QString &name)
{
    bool ok = false;
    ulong opt = name.toULong(&ok, 16);

    if (!ok) {
        return convertError(name);
    } else {
        return tcpoptnameMap.value(opt, QStringLiteral("unknown-tcpopt-name(0x%1)").arg(name));
    }
}

QString AuditConvertor::convertUdpOptName(const QString &name)
{
    bool ok = false;
    int opt = name.toULong(&ok, 16);

    if (!ok) {
        return convertError(name);
    }

    if (opt == 1) {
        return QStringLiteral("UDP_CORK");
    } else if (opt == 100) {
        return QStringLiteral("UDP_ENCAP");
    } else {
        return QStringLiteral("unknown-udpopt-name(0x%1)").arg(name);
    }
}

QString AuditConvertor::convertIp6OptName(const QString &name)
{
    bool ok = false;
    ulong opt = name.toULong(&ok, 16);

    if (!ok) {
        return convertError(name);
    } else {
        return ip6optnameMap.value(opt, QStringLiteral("unknown-ip6opt-name(0x%1)").arg(name));
    }
}

QString AuditConvertor::convertPktOptName(const QString &name)
{
    bool ok = false;
    ulong opt = name.toULong(&ok, 16);

    if (!ok) {
        return convertError(name);
    } else {
        return pktoptnameMap.value(opt, QStringLiteral("unknown-pktopt-name(0x%1)").arg(name));
    }
}

QString AuditConvertor::convertSocketProto(const QString &proto)
{
    bool ok = false;
    unsigned int val = proto.toULong(&ok, 16);

    if (!ok) {
        return convertError(proto);
    }

    struct protoent *p = getprotobynumber(val);

    if (p) {
        return QString::fromLatin1(p->p_name);
    } else {
        return QStringLiteral("unknown-proto(%1)").arg(proto);
    }
}

QString AuditConvertor::convertRecv(const QString &recv)
{
    bool ok = false;
    ulong rec = recv.toULong(&ok, 16);
    int cnt = 0;
    QString retValue;

    if (!ok) {
        return convertError(recv);
    }

    auto it = recvMap.constBegin();
    while (it != recvMap.constEnd()) {
        if (it.key() & rec) {
            if (cnt) {
                retValue += QLatin1String("|");
            }

            retValue += it.value();
            cnt++;
        }
        ++it;
    }

    if (retValue.isEmpty()) {
        return QStringLiteral("0x%1").arg(recv);
    } else {
        return retValue;
    }
}

QString AuditConvertor::convertShmFlags(const QString &shmflags)
{
    bool ok = false;
    ulong flags = shmflags.toULong(&ok, 16);
    ulong partial;
    int cnt = 0;
    QString retValue;

    if (!ok) {
        return convertError(shmflags);
    }

    partial = flags & 00003000;

    auto it = ipccmdMap.constBegin();
    while (it != ipccmdMap.constEnd()) {
        if (it.key() & partial) {
            if (cnt) {
                retValue += QLatin1String("|");
            }

            retValue += it.value();
            cnt++;
        }
        ++it;
    }

    partial = flags & 00014000;

    auto it1 = shmmodeMap.constBegin();
    while (it1 != shmmodeMap.constEnd()) {
        if (it1.key() & partial) {
            if (cnt) {
                retValue += QLatin1String("|");
            }

            retValue += it1.value();
            cnt++;
        }
        ++it1;
    }

    partial = flags & 000777;

    QString tmode = convertModeShortInt(partial);

    if (!tmode.isEmpty()) {
        if (!retValue.isEmpty()) {
            retValue += QLatin1String("|");
        }
        return retValue + tmode;
    } else {
        return QStringLiteral("0x") + shmflags;
    }
}

QString AuditConvertor::convertProt(const QString &proto, unsigned int is_mmap)
{
    bool ok = false;
    ulong prot = proto.toULong(&ok, 16);
    int cnt = 0;
    QString retValue;

    if (!ok) {
        return convertError(proto);
    }

    if ((prot & 0x07) == 0) {
        return QStringLiteral("PROT_NONE");
    }

    auto it = protMap.constBegin();
    auto endIt = protMap.constEnd();

    if (!is_mmap) {
        endIt--;
    }

    while (it != endIt) {
        if (it.key() & prot) {
            if (cnt) {
                retValue += QLatin1String("|");
            }

            retValue += it.value();
            cnt++;
        }
        ++it;
    }

    if (retValue.isEmpty()) {
        return QStringLiteral("0x%1").arg(proto);
    } else {
        return retValue;
    }
}

QString AuditConvertor::convertSeek(const QString &seek)
{
    bool ok = false;
    unsigned int whence = 0xFF & seek.toULong(&ok, 16);

    if (!ok) {
        return convertError(seek);
    } else {
        return seekMap.value(whence, QStringLiteral("unknown-whence(%1)").arg(seek));
    }
}

QString AuditConvertor::convertMmap(const QString &mmap)
{
    bool ok = false;
    ulong maps = mmap.toULong(&ok, 16);
    int cnt = 0;
    QString retValue;

    if (!ok) {
        return convertError(mmap);
    }

    if ((maps & 0x0F) == 0) {
        retValue = QLatin1String("MAP_FILE");
        cnt++;
    }

    auto it = mmapMap.constBegin();
    while (it != mmapMap.constEnd()) {
        if (it.key() & maps) {
            if (cnt) {
                retValue += QLatin1String("|");
            }

            retValue += it.value();
            cnt++;
        }
        ++it;
    }

    if (retValue.isEmpty()) {
        return QStringLiteral("0x%1").arg(mmap);
    } else {
        return retValue;
    }
}

QString AuditConvertor::convertMount(const QString &mnt)
{
    bool ok = false;
    ulong mounts = mnt.toULong(&ok, 16);
    int cnt = 0;
    QString retValue;

    if (!ok) {
        return convertError(mnt);
    }

    auto it = mmapMap.constBegin();
    while (it != mmapMap.constEnd()) {
        if (it.key() & mounts) {
            if (cnt) {
                retValue += QLatin1String("|");
            }

            retValue += it.value();
            cnt++;
        }
        ++it;
    }

    if (retValue.isEmpty()) {
        return QStringLiteral("0x%1").arg(mnt);
    } else {
        return retValue;
    }
}

QString AuditConvertor::convertError(const QString &val)
{
    return QStringLiteral("conversion error(%1)").arg(val);
}
