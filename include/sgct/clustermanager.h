/*****************************************************************************************
 * SGCT                                                                                  *
 * Simple Graphics Cluster Toolkit                                                       *
 *                                                                                       *
 * Copyright (c) 2012-2019                                                               *
 * For conditions of distribution and use, see copyright notice in sgct.h                *
 ****************************************************************************************/

#ifndef __SGCT__CLUSTER_MANAGER__H__
#define __SGCT__CLUSTER_MANAGER__H__

#include <sgct/networkmanager.h>
#include <sgct/node.h>
#include <glm/glm.hpp>
#include <memory>
#include <string>

namespace sgct::config { struct Cluster; }

namespace sgct::core {

class User;

/**
 * The ClusterManager manages all nodes and cluster settings. This class is a static
 * singleton and is accessed using it's instance.
 */
class ClusterManager {
public:
    static ClusterManager& instance();
    static void destroy();

    void applyCluster(const config::Cluster& cluster);

    /// Add a cluster node to the manager's vector.
    void addNode(Node node);

    /// Add a new user.
    void addUser(User userPtr);

    /**
     * Get a pointer to a specific node. Please observe that the address of this object
     * might change between frames and should not be kept around for long.
     *
     * \param int the index to a node in the vector
     * \return the pointer to the requested node or nullptr if not found. This pointer is
     *         not guaranteed to be stable between function calls
     */
    Node* getNode(int index);

    /**
     * Get the current node. Please observe that the address of this object might change
     * between frames and should not be kept around for long.
     *
     * \return a reference to the node that this application is running on
     */
    Node& getThisNode();

    /**
     * Get the default user. Please observe that the address of this object might change
     * between frames and should not be kept around for long.
     *
     * \return the pointer to the default user
     */
    User& getDefaultUser();

    /**
     * Get the user with the specific name. Please observe that the address of this object
     * might change between frames and should not be kept around for long.
     *
     * \return the pointer to a named user. nullptr is returned if no user is found.
     */
    User* getUser(const std::string& name);

    /**
     * Get the tracked user. Please observe that the address of this object might change
     * between frames and should not be kept around for long.
     *
     * \return the pointer to the tracked user. Returns nullptr if no user is tracked.
     */
    User* getTrackedUser();

    /// \return the current network mode
    NetworkManager::NetworkMode getNetworkMode() const;

    /// Sets the current network mode
    void setNetworkMode(NetworkManager::NetworkMode nm);

    /// \return the number of nodes in the cluster
    int getNumberOfNodes() const;
    
    /// \return the scene transform specified in the configuration file
    const glm::mat4& getSceneTransform() const;

    /**
     * Don't set this, this is done automatically using from the Network Manager which
     * compares IP addresses from this computer to the XML config file.
     *
     * \param the index to the node where this application is running on
     */
    void setThisNodeId(int id);

    /// \return the id to the node which runs this application
    int getThisNodeId() const;

    /// \return the dns, name or IP of the master in the cluster
    const std::string& getMasterAddress() const;

    /// \return state of the firm frame lock lock sync
    bool getFirmFrameLockSyncStatus() const;

    /// \param the state of the firm frame lock sync
    void setFirmFrameLockSyncStatus(bool state);

    /// \return the external control port number
    int getExternalControlPort() const;

    /// \param the external control port number
    void setExternalControlPort(int port);

    /// Set if external control should use ASCII (Telnet) or raw binary parsing.
    void setUseASCIIForExternalControl(bool useASCII);

    /// Get if external control is using ASCII (Telnet) or raw binary parsing.
    bool getUseASCIIForExternalControl() const;

    /// Set if software sync between nodes should be ignored
    void setUseIgnoreSync(bool state);

    /// Get if software sync between nodes is disabled
    bool getIgnoreSync() const;

    /// Set the scene transform.
    void setSceneTransform(glm::mat4 mat);

    /**
     * Set the scene offset/translation. This is set using the XML config file for easier
     * transitions between different hardware setups.
     */
    void setSceneOffset(glm::vec3 offset);

    /**
     * Set the scene rotation. This is set using the XML config file for easier
     * transitions between different hardware setups.
     */
    void setSceneRotation(float yaw, float pitch, float roll);

    /**
     * Set the scene rotation. This is set using the XML config file for easier
     * transitions between different hardware setups.
     */
    void setSceneRotation(glm::mat4 mat);
    
    /**
     * Set the scene scale. This is set using the XML config file for easier transitions
     * between different hardware setups.
     */
    void setSceneScale(float scale);

private:
    ClusterManager();

    static ClusterManager* _instance;

    std::vector<Node> _nodes;

    int _thisNodeId = -1;
    bool _firmFrameLockSync = false;
    bool _ignoreSync = false;
    std::string _masterAddress;
    int _externalControlPort = 0;
    bool _useASCIIForExternalControl = true;

    std::vector<User> _users;

    glm::mat4 _sceneTransform = glm::mat4(1.f);
    glm::mat4 _sceneScale = glm::mat4(1.f);
    glm::mat4 _sceneTranslate = glm::mat4(1.f);
    glm::mat4 _sceneRotation = glm::mat4(1.f);
    NetworkManager::NetworkMode _netMode = NetworkManager::NetworkMode::Remote;
};

} // namespace sgct::core

#endif // __SGCT__CLUSTER_MANAGER__H__
