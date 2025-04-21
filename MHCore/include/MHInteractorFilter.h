#pragma once

/**
 * @file MHInteractorFilter.h
 * @author liuyulvv (liuyulvv@outlook.com)
 * @date 2025-03-18
 */

#ifdef MH_CORE_EXPORTS
#define MH_CORE_API __declspec(dllexport)
#else
#define MH_CORE_API __declspec(dllimport)
#endif

#include <set>
#include <string>

namespace MHCore {

struct MH_CORE_API MHInteractorInfo {
    int screenX;
    int screenY;
    double worldX;
    double worldY;
    std::set<std::string> pressedKeys;
};

class MH_CORE_API MHInteractorFilter {
public:
    MHInteractorFilter() = default;
    virtual ~MHInteractorFilter() = default;
    MHInteractorFilter(const MHInteractorFilter& filter) = delete;
    MHInteractorFilter(MHInteractorFilter&& filter) = delete;
    MHInteractorFilter& operator=(const MHInteractorFilter& filter) = delete;
    MHInteractorFilter& operator=(MHInteractorFilter&& filter) = delete;

public:
    virtual int getPriority() const = 0;
    virtual bool onLeftButtonDown(const MHInteractorInfo& interactorInfo);
    virtual bool onLeftButtonUp(const MHInteractorInfo& interactorInfo);
    virtual bool onRightButtonDown(const MHInteractorInfo& interactorInfo);
    virtual bool onRightButtonUp(const MHInteractorInfo& interactorInfo);
    virtual bool onMouseMove(const MHInteractorInfo& interactorInfo);
    virtual bool onKeyPress(const MHInteractorInfo& interactorInfo);
    virtual bool onKeyRelease(const MHInteractorInfo& interactorInfo);
};

}  // namespace MHCore
