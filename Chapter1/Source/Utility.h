#pragma once

//===============================================
//
// äÓñ{
//
//===============================================
#pragma comment(lib,"winmm.lib")

#include <windows.h>
#include <stdio.h>
#include <math.h>
#include <commctrl.h>
#include <ctime>
#include <wrl.h>
#include <wrl/client.h>
#include <cassert>


//===============================================
//
// STL
//
//===============================================
#include <map>
#include <unordered_map>
#include <map>
#include <string>
#include <array>
#include <vector>
#include <stack>
#include <list>
#include <iterator>
#include <queue>
#include <algorithm>
#include <memory>
#include <random>
#include <fstream>
#include <iostream>
#include <sstream>
#include <functional>
#include <thread>
#include <atomic>
#include <mutex>
#include <future>

namespace origin
{
namespace chack {
template <class T>
bool Unique(const std::shared_ptr<T>& aPtr) { return aPtr.use_count() == 1; }
}

template<class T>
using Uptr = std::unique_ptr<T>;

template<class T>
using Sptr = std::shared_ptr<T>;

template<class T>
using Wptr = std::weak_ptr<T>;

template<class T>
using ComPtr = Microsoft::WRL::ComPtr<T>;

//make_sharedÇè»ó™
template<class T>
inline Sptr<T> MakeSptr() { return std::make_shared<T>(); }
}