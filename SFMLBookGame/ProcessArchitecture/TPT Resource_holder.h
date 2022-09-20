#pragma once

#include "../pch.h"

//declarations
template<typename Resource, typename Identifier>
class Resource_holder {
public:
    void load(Identifier iid, const std::string& ffilename);
    template <typename Extraparam>
    void load(Identifier iid, const std::string& ffilename, const Extraparam& eextra_param); //for sf::Shaders
    Resource& get_resource(Identifier iid);
    const Resource& get_resource(Identifier iid) const;
private:
    std::map<Identifier, std::unique_ptr<Resource>> resource_map;
};

//definitions
template<typename Resource, typename Identifier>
void Resource_holder<Resource, Identifier>::load(Identifier iid, const std::string& ffilename)
{
    //create and load resource
    std::unique_ptr<Resource> asset {new Resource {}};
    if(!asset->loadFromFile(ffilename)) {
        throw std::runtime_error {"Resource_holder::load failed - " + ffilename};
    }
    resource_map.insert(std::make_pair(iid, std::move(asset)));
}

template<typename Resource, typename Identifier>
template<typename Extraparam>
void Resource_holder<Resource, Identifier>::load(Identifier iid, const std::string& ffilename, const Extraparam& eextra_param)
{
    std::unique_ptr<Resource> asset {new Resource {}};
    if(!asset->loadFromFile(ffilename, eextra_param)) {
        throw std::runtime_error {"Resource_holder::load failed - " + ffilename};
    }
    resource_map.insert(std::make_pair(iid, std::move(asset)));
}

template<typename Resource, typename Identifier>
Resource& Resource_holder<Resource, Identifier>::get_resource(Identifier iid)
{
    auto iterator {resource_map.find(iid)};
    if(iterator == resource_map.end()) {
        throw std::logic_error {"Resource_holder::get_resource"};
    }
    return *(iterator->second);
}

template<typename Resource, typename Identifier>
const Resource& Resource_holder<Resource, Identifier>::get_resource(Identifier iid) const
{
    auto iterator {resource_map.find(iid)};
    if(iterator == resource_map.end()) {
        throw std::logic_error {"Resource_holder::get_resource"};
    }
    return *(iterator->second);
}
