#pragma once

#include <gltk/Crash.h>

#define gltk_Check(p) do { if (!(p)) gltk_Crash("Check failed: '" #p "'"); } while (false)
