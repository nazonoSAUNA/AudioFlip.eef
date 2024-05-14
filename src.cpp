#include <windows.h>
#include <exedit.hpp>

#define CHECK_N 2

inline static char name[] = "反転(音声)";
inline static char* check_name[CHECK_N] = { const_cast<char*>("左右"), const_cast<char*>("位相") };
inline static int check_default[CHECK_N] = { 0, 0 };

BOOL func_proc(ExEdit::Filter* efp, ExEdit::FilterProcInfo* efpip) {
    short* audiop;
    if ((int)efp->flag & (int)ExEdit::Filter::Flag::Effect) {
        audiop = efpip->audio_data;
    } else {
        audiop = efpip->audio_p;
    }

    if (efp->check[0] && efpip->audio_ch == 2) {
        auto ptr = audiop;
        for (int i = efpip->audio_n; 0 < i; i--) {
            std::swap(ptr[0], ptr[1]);
            ptr += 2;
        }
    }
    if (efp->check[1]) {
        /*
        auto ptr = audiop;
        for (int i = efpip->audio_n * efpip->audio_ch; 0 < i; i--) {
            *ptr = ~*ptr;
            ptr++;
        }
        */
        int* ptr = reinterpret_cast<int*>(audiop);
        for (int i = (efpip->audio_n * efpip->audio_ch + 1) >> 1; 0 < i; i--) {
            *ptr = ~*ptr;
            ptr++;
        }
    }

    return TRUE;
}


ExEdit::Filter effect_ef = {
    .flag = ExEdit::Filter::Flag::Audio | ExEdit::Filter::Flag::Effect,
    .name = name,
    .track_n = 0,
    .check_n = CHECK_N,
    .check_name = check_name,
    .check_default = check_default,
    .func_proc = &func_proc,
};
ExEdit::Filter filter_ef = {
    .flag = ExEdit::Filter::Flag::Audio,
    .name = name,
    .track_n = 0,
    .check_n = CHECK_N,
    .check_name = check_name,
    .check_default = check_default,
    .func_proc = &func_proc,
};

ExEdit::Filter* filter_list[] = {
    &effect_ef, &filter_ef,
    NULL
};
EXTERN_C __declspec(dllexport)ExEdit::Filter** __stdcall GetFilterTableList() {
    return filter_list;
}
