#ifndef _SERVER_ABSTRACT_PORT_H
#define _SERVER_ABSTRACT_PORT_H

#include <QList>
#include <QtGlobal>

#include "../common/protocol.pb.h"

class StreamBase;
class QIODevice;

class AbstractPort
{
public:
    struct PortStats
    {
        quint64    rxPkts;
        quint64    rxBytes;
        quint64    rxPps;
        quint64    rxBps;

        quint64    txPkts;
        quint64    txBytes;
        quint64    txPps;
        quint64    txBps;
    };

    AbstractPort(int id, const char *device);
    virtual ~AbstractPort();

    virtual void init();

    int id() { return data_.port_id().id(); }
    void protoDataCopyInto(OstProto::Port *port) { port->CopyFrom(data_); }

    int streamCount() { return streamList_.size(); }
    StreamBase* stream(int streamId);
    bool addStream(StreamBase *stream);
    bool deleteStream(int streamId);

    bool isDirty() { return isSendQueueDirty_; }
    void setDirty() { isSendQueueDirty_ = true; }

    virtual OstProto::LinkState linkState() { return linkState_; }

    virtual void clearPacketList() = 0;
    virtual bool appendToPacketList(long sec, long usec, const uchar *packet, 
            int length) = 0;
    virtual void setPacketListLoopMode(bool loop) = 0;
    void updatePacketList();

    virtual void startTransmit() = 0;
    virtual void stopTransmit() = 0;
    virtual bool isTransmitOn() = 0;

    virtual void startCapture() = 0;
    virtual void stopCapture() = 0;
    virtual bool isCaptureOn() = 0;
    virtual QIODevice* captureData() = 0;

    void stats(PortStats *stats);
    void resetStats() { epochStats_ = stats_; }

protected:
    OstProto::Port          data_;
    OstProto::LinkState     linkState_;

    struct PortStats    stats_;
    //! \todo Need lock for stats access/update

private:
    bool    isSendQueueDirty_;
    /*! \note StreamBase::id() and index into streamList[] are NOT same! */
    QList<StreamBase*>  streamList_;

    struct PortStats    epochStats_;

};

#endif