/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
#include "LoadedResourcesHolderImpl.hpp"
namespace Resources
{
ResourceHolderPtr resourceHolderPtr;

bool isResourceHolderCreated()
{
    return resourceHolderPtr != nullptr;
}

ResourceHolder *getResourceHolderFacility()
{
    return resourceHolderPtr.get();
}
}
