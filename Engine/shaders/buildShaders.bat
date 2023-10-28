@echo off
setlocal enabledelayedexpansion
set glslc=%1

set "script_directory=%~dp0"

for %%f in ("%script_directory%frag\*") do (
    if "%%~xf"==".frag" (
        call %glslc% %%f -o %script_directory%frag\%%~nf.spv
    )
)

for %%f in ("%script_directory%vert\*") do (
    if "%%~xf"==".vert" (
        call %glslc% %%f -o %script_directory%vert\%%~nf.spv
    )
)