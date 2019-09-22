/*************************************************************************
Copyright (c) 2012-2015 Miroslav Andel
All rights reserved.

For conditions of distribution and use, see copyright notice in sgct.h 
*************************************************************************/

#ifndef __SGCT__NETWORK_MANAGER__H__
#define __SGCT__NETWORK_MANAGER__H__

#include <sgct/network.h>
#include <atomic>
#include <string>
#include <vector>

namespace sgct::core {

class Statistics;

/**
 * The network manager manages all network connections for SGCT.
 */
class NetworkManager {
public:
    enum class SyncMode { SendDataToClients = 0, AcknowledgeData };
    enum class NetworkMode { Remote = 0, LocalServer, LocalClient };

    static std::condition_variable gCond;

    NetworkManager(NetworkMode nm);
    ~NetworkManager();
    bool init();

    ///  \param if this application is server/master in cluster then set to true
    void sync(SyncMode sm, Statistics& statsPtr);

    /**
     * Compare if the last frame and current frames are different -> data update
     * And if send frame == recieved frame
     */
    bool isSyncComplete();
    void close();

    /// \returns the static pointer to the NetworkManager instance
    static NetworkManager* instance();

    bool matchAddress(const std::string& address);

    /// Retrieve the node id if this node is part of the cluster configuration
    void retrieveNodeId();
    bool isComputerServer();
    bool isRunning();
    bool areAllNodesConnected();
    Network* getExternalControlPtr();
    void transferData(const void* data, int length, int packageId);
    void transferData(const void* data, int length, int packageId, size_t nodeIndex);
    void transferData(const void* data, int length, int packageId, Network* connection);

    /**
     * Compression levels 1-9.
     *   -1 = Default compression
     *    0 = No compression
     *    1 = Best speed
     *    9 = Best compression
     */
    void setDataTransferCompression(bool state, int level = 1);

    unsigned int getActiveConnectionsCount();
    unsigned int getActiveSyncConnectionsCount();
    unsigned int getActiveDataTransferConnectionsCount();
    int getConnectionsCount();
    int getSyncConnectionsCount();
    int getDataTransferConnectionsCount();
    const Network& getConnectionByIndex(unsigned int index) const;
    Network* getSyncConnectionByIndex(unsigned int index) const;
    const std::vector<std::string>& getLocalAddresses() const;

private:
    bool addConnection(int port, const std::string& address,
        Network::ConnectionType connectionType = Network::ConnectionType::SyncConnection);
    void initAPI();
    void getHostInfo();
    void updateConnectionStatus(Network* connection);
    void setAllNodesConnected();
    bool prepareTransferData(const void* data, std::vector<char>& buffer, int& length,
        int packageId);

    static NetworkManager* mInstance;

    // This could be a std::vector<Network>, but Network is not move-constructible
    // because of the std::condition_variable in it
    std::vector<std::unique_ptr<Network>> mNetworkConnections;
    std::vector<Network*> mSyncConnections;
    std::vector<Network*> mDataTransferConnections;
    Network* mExternalControlConnection = nullptr;

    std::string mHostName; // stores this computers hostname
    std::vector<std::string> mDNSNames;
    std::vector<std::string> mLocalAddresses; // stores this computers ip addresses

    bool mIsServer = true;
    bool mIsRunning = true;
    bool mAllNodesConnected = false;
    std::atomic_bool mCompress = false;
    std::atomic_int mCompressionLevel;
    NetworkMode mMode;
    unsigned int mNumberOfActiveConnections = 0;
    unsigned int mNumberOfActiveSyncConnections = 0;
    unsigned int mNumberOfActiveDataTransferConnections = 0;
};

} // namespace sgct::core

#endif // __SGCT__NETWORK_MANAGER__H__
