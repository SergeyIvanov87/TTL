#ifndef RESOURCE_TRAITS_H
#define RESOURCE_TRAITS_H

template<class Resource>
struct ResourcesTraits
{
    static constexpr const char* getResourcePath() noexcept {return "";}
    static constexpr bool hasAssetsPath() noexcept {return false;}
    static constexpr const char *getResourceRegexFilter() noexcept {return ".*";}
};
#endif //RESOURCE_TRAITS_H
