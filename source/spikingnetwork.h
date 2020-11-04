#ifndef SPIKINGNETWORK_H
#define SPIKINGNETWORK_H

#include <cypress/cypress.hpp>
//#include <fstream>
//#include <iostream>
#include <cypress/cypress.hpp>
using json = cypress::Json;
using namespace cypress;

struct triggerType{
  int value;
  int startTime;
};

struct triggerType2{
  std::vector<bool> value;
  int startTime;
};

struct parameterType{
    NeuronParameter neuroParams;
    NeuronParameter neuroParams2;
    NeuronParameter neuroParams3;
    Real delay;
    Real start;
    Real wtaSynEx;
    Real wtaSynIn;
    Real wtaSynTrigger;
    Real stSynEx;
    Real stSynIn;
    Real psWeight;
    Real psDelay;
    Real psAMinus;
    Real psAPlus;
    Real psTauMinus;
    Real psTauPlus;
    Real psWMax;
    Real psWMin;
    Real conSynEx;
    Real conDelay;
    Real posSynEx;
};

class SpikingNetwork
{
private:
    Real runtime;
    Network m_netw;
    parameterType parameter;
    size_t maxAV,maxV,xSize,ySize,numDirections;
    const char *simulator;

    void createIntWTA(Network& netw, std::string name,size_t size,size_t maxShift,bool overflow);
    void createPositionNetwork(Network& netw);
    int plotPopulation(Network& netw, std::string name, int i, int time);
    int plotMembranVoltage(Network& netw, std::string name, int neuron, int i);
    int plotWeights(Network& netw, int i);
    void createCollisionDetectionNetwork(Network& netw);
    void createTrigger(Network& netw, std::string name,size_t maxShift,std::vector<triggerType> trigger);
    void createTrigger(Network& netw, std::string name,std::vector<triggerType2> trigger);
    void createMoveDirNet(Network& netw, int maxV);
    parameterType readParameters(std::string config_path);
    parameterType readParameters(const Json& config);

public:
    SpikingNetwork(const char *simulator= "nest",size_t numDirections=8, size_t xSize=3, size_t ySize=3, size_t maxAV=1, size_t maxV=1, std::string config_path = "");
    SpikingNetwork(const char *simulator, const Json& config, size_t numDirections=8, size_t xSize=3, size_t ySize=3, size_t maxAV=1, size_t maxV=1);
    void createNetwork(Network& netw, std::string sim_path= "../result/simulation");
    void createNetwork(std::string sim_path= "../result/simulation");
    void run();
    void printResults();
    json readFromFile(std::string directory);
};

class AllbutONEConnectorMat : public UniformConnector {
public:
    //using UniformConnector::UniformConnector;
    AllbutONEConnectorMat(Real weight=0.0, Real delay=0.0, std::string direction="x", bool self_connections=true);
    ~AllbutONEConnectorMat() override = default;
    void connect(const ConnectionDescriptor &descr,
        std::vector<LocalConnection> &tar) const override;
    bool group_connect(const ConnectionDescriptor &) const override
        {
            return true;
        }
    bool valid(const ConnectionDescriptor &) const override { return true; }
    size_t size(size_t size_src_pop, size_t size_target_pop) const override
        {
            return size_src_pop * size_target_pop;
        }
    std::string name() const override { return "AllbutOneConnectorMat"; }
private :
    std::string direction;
};

class AllbutONEConnectorVec : public UniformConnector {
public:
    using UniformConnector::UniformConnector;
    ~AllbutONEConnectorVec() override = default;
    void connect(const ConnectionDescriptor &descr,
        std::vector<LocalConnection> &tar) const override;
    bool group_connect(const ConnectionDescriptor &) const override
        {
            return true;
        }
    bool valid(const ConnectionDescriptor &) const override { return true; }
    size_t size(size_t size_src_pop, size_t size_target_pop) const override
        {
            return size_src_pop * size_target_pop;
        }
    std::string name() const override { return "AllbutOneConnectorVec"; }
};

class ShiftConnector : public UniformConnector {
public:
    //using UniformConnector::UniformConnector;
    ShiftConnector(Real weight=0.0, Real delay=0.0, signed shift=0,bool overflow=false, bool self_connections=true);
    ~ShiftConnector() override = default;
    void connect(const ConnectionDescriptor &descr,
        std::vector<LocalConnection> &tar) const override;
    bool group_connect(const ConnectionDescriptor &) const override
        {
            return true;
        }
    bool valid(const ConnectionDescriptor &) const override { return true; }
    size_t size(size_t size_src_pop, size_t size_target_pop) const override
        {
            return size_src_pop * size_target_pop;
        }
    std::string name() const override { return "ShiftConnector"; }
private:
    signed shift;
    bool overflow;
};

class ShiftConnectorMat : public UniformConnector {
public:
    //using UniformConnector::UniformConnector;
    ShiftConnectorMat(Real weight=0.0, Real delay=0.0, signed shift=0,bool overflow=false, bool self_connections=true);
    ~ShiftConnectorMat() override = default;
    void connect(const ConnectionDescriptor &descr,
        std::vector<LocalConnection> &tar) const override;
    bool group_connect(const ConnectionDescriptor &) const override
        {
            return true;
        }
    bool valid(const ConnectionDescriptor &) const override { return true; }
    size_t size(size_t size_src_pop, size_t size_target_pop) const override
        {
            return size_src_pop * size_target_pop;
        }
    std::string name() const override { return "ShiftConnectorMat"; }
private:
    signed shift_;
    bool overflow;
};

class OneToVecConnector : public UniformConnector {
public:
    //using UniformConnector::UniformConnector;
    OneToVecConnector(Real weight=0.0, Real delay=0.0, std::string direction="x", bool self_connections=true);
    ~OneToVecConnector() override = default;
    void connect(const ConnectionDescriptor &descr,
        std::vector<LocalConnection> &tar) const override;
    bool group_connect(const ConnectionDescriptor &) const override
        {
            return true;
        }
    bool valid(const ConnectionDescriptor &) const override { return true; }
    size_t size(size_t size_src_pop, size_t size_target_pop) const override
        {
            return size_src_pop * size_target_pop;
        }
    std::string name() const override { return "OneToVecConnector"; }
private:
    std::string direction;
};

class CustomConnector : public UniformConnector {
public:
    //using UniformConnector::UniformConnector;
    CustomConnector(Real weight=0.0, Real delay=0.0, std::vector<std::vector<NeuronIndex>> target_list={}, bool self_connections=true);
    ~CustomConnector() override = default;
    void connect(const ConnectionDescriptor &descr,
        std::vector<LocalConnection> &tar) const override;
    bool group_connect(const ConnectionDescriptor &) const override
        {
            return true;
        }
    bool valid(const ConnectionDescriptor &) const override { return true; }
    size_t size(size_t size_src_pop, size_t size_target_pop) const override
        {
            return size_src_pop * size_target_pop;
        }
    std::string name() const override { return "CustomConnector"; }
private:
    std::vector<std::vector<NeuronIndex>> target_list;
};
#endif // SPIKINGNETWORK_H
