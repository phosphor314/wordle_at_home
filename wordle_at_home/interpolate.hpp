#pragma once

#include <concepts>
#include <algorithm>
#include <functional>
#include <chrono>
#include <math.h>


template <typename T>
concept Vector = requires(T a, T b, float x){
    a + b;
    a - b;
    a * x;
};

namespace easingFuns{
    struct Linear{
        float operator()(float x) const{
            return x;
        }
    };
    
    struct Smoothstep{
        float operator()(float x) const{
            float t = std::clamp(x, 0.0f, 1.0f);
            return 3*t*t-2*t*t*t;
        }
    };
    
    struct Smootherstep{
        float operator()(float x) const{
            float t = std::clamp(x, 0.0f, 1.0f);
            return 6*t*t*t*t*t - 15*t*t*t*t + 10*t*t*t;
        }
    };
    
    /*struct EaseOutElastic {
        float operator()(float x) const{
            const float c4 = (2 * std::numbers::pi / 3);

  			return std::pow(2, -10 * x) * std::sin((x * 10 - 0.75) * c4) + 1;
        }
    };*/
    
    struct EasinInOutExpo{
        float operator()(float x) const{
            return x < 0.5 ? std::pow(2, 20 * x - 10) / 2 : (2 - std::pow(2, -20 * x + 10)) / 2;
        }
    };
    
    struct EaseOutExpo{
        float operator()(float x) const{
            return 1 - std::pow(2, -10*x);
        }
    };
}

template<Vector T, typename easingFun = easingFuns::Linear>
requires std::invocable<easingFun, float> && std::same_as<std::invoke_result_t<easingFun, float>, float>
struct Interpolated{
public:
	float transition_speed;
	
	Interpolated() = default;

    explicit Interpolated(T val, float transition_speed_s = 1.0f, bool clamp = true){
        this->begin = val;
        this->end = val;
        this->transition_speed = transition_speed_s;
    }
    
    operator T(){
        Fsec elapsed = Clock::now() - t;
        float param;
        if (transition_speed == 0.0f){
            param = 1.0f;
        }
        else{
            param = std::clamp(elapsed.count(), 0.0f, 1.0f) / transition_speed;
        }
        return begin + (end - begin)*std::invoke(easingFun{}, param);
    }
    
    void setTarget(T target){
        if (end == target){return;}
        begin = (T)*this;
        end = target;
        t = Clock::now();
    }
    
    void forceSet(T val){
        begin = val;
        end = val;
    }
private:
    typedef std::chrono::high_resolution_clock Clock;
    typedef std::chrono::duration<float> Fsec;
	
	T begin;
	T end;
	Clock::time_point t = Clock::now();
};
