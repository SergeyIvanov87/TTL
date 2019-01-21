#ifndef CUSTOM_RESOURCE_LOADERS_H
#define CUSTOM_RESOURCE_LOADERS_H
extern const char system_info_dummy_path[];

#include <Framework/ResourcesFramework/IBaseResource.h>
#include <Framework/Utils/StringUtils.h>
#include <Framework/SerializeFramework/Interfaces/ISerializable.h>

class ResourceA : public Resources::IBaseResource<ResourceA>, public ISerializable<ResourceA>
{
public:
    using Base = Resources::IBaseResource<ResourceA>;
    using Base::ResourcesMap;
    static constexpr size_t instancesForLoadingCount = 10;
    enum
    {
        isSerializableSupport = false,
        isDumpObjectSupport = false
    };


    ResourceA(const std::string &name) :
        Base(),
        m_name(name)
    {
    }

    //Interfaces Decl
    template<class Tracer>
    static ResourcesMap loadResourcesImpl(const std::string &resourcePath, Tracer tracer)
    {
        ResourcesMap instances;
        for (size_t i = 0; i < instancesForLoadingCount; i ++)
        {
            std::string aggregatedName = makeString(resourcePath + getResourceTypeDescriptionImpl(), "_", i);
            instances.emplace(aggregatedName, std::make_shared<ResourceA>(aggregatedName));
        }
        return instances;
    }

    constexpr static const char *getResourceTypeDescriptionImpl()
    {
        return "ResourceA";
    }

    std::string m_name;
};



class ResourceB : public Resources::IBaseResource<ResourceB>, public ISerializable<ResourceB>
{
public:
    using Base = Resources::IBaseResource<ResourceB>;
    using Base::ResourcesMap;
    static constexpr size_t instancesForLoadingCount = 6;
    enum
    {
        isSerializableSupport = true,
        isDumpObjectSupport = true
    };

    ResourceB(const std::string &name) :
        Base(),
        m_name(name)
    {
    }

    //Interfaces Decl
    template<class Tracer>
    static ResourcesMap loadResourcesImpl(const std::string &resourcePath, Tracer tracer)
    {
        ResourcesMap instances;
        for (size_t i = 0; i < instancesForLoadingCount; i ++)
        {
            std::string aggregatedName = makeString(resourcePath + getResourceTypeDescriptionImpl(), "_", i);
            instances.emplace(aggregatedName, std::make_shared<ResourceB>(aggregatedName));
        }
        return instances;
    }

    constexpr static const char *getResourceTypeDescriptionImpl()
    {
        return "ResourceB";
    }

    //Impl
    bool serializeImpl(std::ostream &out)
    {
        serializeParams(out, m_name);

        m_name.clear();
        m_name.shrink_to_fit();
        return true;
    }

    bool deserializeImpl(std::istream &out)
    {
        deserializeParams(out, m_name);
        return true;
    }

    size_t getObjectSizeImpl() const
    {
        return m_name.size();
    }
    std::string m_name;
};

//Resource Traits For Resources
#include <Framework/ResourcesFramework/ResourceTraits.h>
template<>
struct ResourcesTraits<ResourceA>
{
   static constexpr const char* getResourcePath() noexcept {return "A_path";}
   static constexpr bool hasAssetsPath() noexcept {return true;}
   static constexpr const char *getResourceRegexFilter() noexcept {return ".*A.*";}
};

template<>
struct ResourcesTraits<ResourceB>
{
   static constexpr const char* getResourcePath() noexcept {return "B_path";}
   static constexpr bool hasAssetsPath() noexcept {return true;}
   static constexpr const char *getResourceRegexFilter() noexcept {return ".*B.*";}
};
#endif //CUSTOM_RESOURCE_LOADERS_H
