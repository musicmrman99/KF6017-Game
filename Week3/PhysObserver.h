#pragma once

#include "PhysModel.h"

class PhysObserver {
private:
    PhysModel::WPtr _physModel;

public:
    using WPtr = std::weak_ptr<PhysObserver>;

    virtual ~PhysObserver() {}
    
    PhysModel::WPtr physModelWPtr() const;
    PhysModel::Ptr physModel() const;

    void updatePhysModel(PhysModel::WPtr physModel);
};
