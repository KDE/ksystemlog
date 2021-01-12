#ifndef AUDITCONVERTOR_H
#define AUDITCONVERTOR_H

#include <QMap>
#include <QString>

class AuditConvertor
{
    using convertParamFunc = QString (AuditConvertor::*)(const QString &);

public:
    AuditConvertor();

    QString convertMessage(const QString &message);

private:
    QString getParam(const QString &message, const QString &name);
    QString replaceParam(const QString &message, const QString &name, const QString &value);

    QString convertProcTitle(const QString &proctitle);
    QString convertArch(const QString &arch);
    QString convertSysCall(const QString &syscall);
    QString convertExit(const QString &exit);
    QString convertUid(const QString &uid, int base);
    QString convertUid(const QString &uid);
    QString convertGid(const QString &gid, int base);
    QString convertGid(const QString &gid);
    QString convertMode(const QString &mode, int base);
    QString convertMode(const QString &mode);
    QString convertA0(const QString &a0);
    QString convertA1(const QString &a1);
    QString convertA2(const QString &a2);
    QString convertA3(const QString &a3);

    QString convertSignals(const QString &sig, unsigned int base);
    QString convertDirFd(const QString &dirfd);
    QString convertCloneFlags(const QString &cloneFlags);
    QString convertClockId(const QString &clockId);
    QString convertPersonality(const QString &personality);
    QString convertPtrace(const QString &ptrace);
    QString convertPrctlOpt(const QString &prctlOpt);
    QString convertRlimit(const QString &rlimit);
    QString convertSocketDomain(const QString &socketDomain);
    QString convertSocketCall(const QString &socketCall, unsigned int base);
    QString convertIpcCall(const QString &ipcCall, unsigned int base);
    QString convertExitSysCall(const QString &exitSysCall);
    QString convertBpf(const QString &bpf);
    QString convertModeShort(const QString &mode, int base);
    QString convertModeShortInt(int mode);
    QString convertFcntlCmd(const QString &cmd);
    QString convertSockOptLevel(const QString &level);
    QString convertSocketType(const QString &type);
    QString convertSched(const QString &sched);
    QString convertOpenFlags(const QString &flags);
    QString convertAccess(const QString &access);
    QString convertEpollCtl(const QString &ctl);
    QString convertCapabilities(const QString &capabilities, int base);
    QString convertUmount(const QString &umount);
    QString convertIoctlReq(const QString &ioctlReq);
    QString convertFcntl(const QString &fcntl);
    QString convertIpOptName(const QString &name);
    QString convertSockOptName(const QString &name);
    QString convertTcpOptName(const QString &name);
    QString convertUdpOptName(const QString &name);
    QString convertIp6OptName(const QString &name);
    QString convertPktOptName(const QString &name);
    QString convertSocketProto(const QString &proto);
    QString convertRecv(const QString &recv);
    QString convertShmFlags(const QString &shmflags);
    QString convertProt(const QString &prot, unsigned int is_mmap);
    QString convertSeek(const QString &seek);
    QString convertMmap(const QString &mmap);
    QString convertMount(const QString &mnt);

    QString convertError(const QString &val);

private:
    QMap<QString, convertParamFunc> m_paramMap;
    QString m_syscall;
    unsigned int m_a0;
    unsigned int m_a1;
};

#endif // AUDITCONVERTOR_H
