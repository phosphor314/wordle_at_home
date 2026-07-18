#pragma once

#include <concepts>
#include <algorithm>
#include <functional>


template <typename T>
concept Vector = requires(T a, T b, float x){
    a + b;
    a - b;
    a * x;
};

namespace easingFuns{
    struct Linear{
        float operator()(float x){
            return x;
        }
    };
    
    struct Smoothstep{
        float operator()(float x){
            float t = std::clamp(x, 0.0f, 1.0f);
            return 3*t*t-2*t*t*t;
        }
    };
    
    struct Smootherstep{
        float operator()(float x){
            float t = std::clamp(x, 0.0f, 1.0f);
            return 6*t*t*t*t*t - 15*t*t*t*t + 10*t*t*t;
        }
    };
}

template<Vector T, typename easingFun = easingFuns::Linear>
requires std::invocable<easingFun, float> && std::same_as<std::invoke_result_t<easingFun, float>, float>
struct Interpolated{
    Interpolated(T begin, T end, float* param){
        this->begin = begin;
        this->end = end;
        this->param = param;
    }
    operator T(){
        return begin + (end - begin)*std::invoke(easingFun{}, *param);
    }
private:
	T begin;
	T end;
	float* param;
};
