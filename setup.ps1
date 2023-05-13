Push-Location $PSScriptRoot
Write-Host "Current working directory: $(Get-Location)"

$vcpkgRoot = $env:VCPKG_ROOT
if ($null -eq $vcpkgRoot -or !(Test-Path $vcpkgRoot -PathType Container)) {
    Write-Host "You need to setup vcpkg and set `$env:VCPKG_ROOT to its path."
    Write-Host "Please refer to: https://vcpkg.io/en/getting-started.html"
    return
}

$vcpkgPackages = @(
    "boost"
    "gtest"
    "opencl"
    "cpu-features"
    "backward-cpp"
    "meshoptimizer"
    "sdl2[vulkan]"
    "sdl2-ttf"
)

$vcpkgTriplet = $null
if ($IsWindows) {
    $vcpkgTriplet = "x64-windows"
}

function install_vcpkg_packages {
    $vcpkgExecutable = "vcpkg"
    if ($IsWindows) {
        $vcpkgExecutable = "vcpkg.exe"
    }
    $vcpkgExecutable = Join-Path -Path $vcpkgRoot -ChildPath $vcpkgExecutable
    if (!(Test-Path -Path $vcpkgExecutable -PathType Leaf)) {
        Write-Host "vcpkg executable does not exist!"
        return
    }

    Push-Location $vcpkgRoot
    Write-Host "Current working directory: $(Get-Location)"
    Write-Host "Updating vcpkg ..."
    Invoke-Expression "git pull"
    Write-Host "Installing vcpkg packages ..."
    foreach ($package in $vcpkgPackages) {
        Write-Host "Installing package $package ..."
        if ($null -ne $vcpkgTriplet) {
            Invoke-Expression "& `"$vcpkgExecutable`" install $package`:$vcpkgTriplet"
        } else {
            Invoke-Expression "& `"$vcpkgExecutable`" install $package"
        }
    }
    Pop-Location
    Write-Host "Current working directory: $(Get-Location)"
}

function build_3rdparty_spvgen {
    $spvgenSourceDir = Join-Path -Path $PSScriptRoot -ChildPath "3rdparty/spvgen/"
    $spvgenExternalDir = Join-Path -Path $spvgenSourceDir -ChildPath "external/"
    $spvgenExternalScript = Join-Path -Path $spvgenExternalDir -ChildPath "fetch_external_sources.py"
    $spvgenBuildDir = Join-Path -Path $PSScriptRoot -ChildPath "3rdparty/build/spvgen"
    Set-location $PSScriptRoot
    Push-Location $spvgenExternalDir
    Invoke-Expression "python `"$spvgenExternalScript`" --targetdir `"$spvgenExternalDir`""
    Pop-Location
    Invoke-Expression "cmake -S `"$spvgenSourceDir`" -B `"$spvgenBuildDir`""
    if (Test-Path -Path $spvgenBuildDir -PathType Container) {
        Invoke-Expression "cmake --build `"$spvgenBuildDir`" --target spvgen --config Release"
    }
}

function setup_radcpp {
    if ($PSVersionTable.PSVersion.Major -lt 7) {
        Write-Host "Current PSVersion: $($PSVersionTable.PSVersion)"
        Write-Host "Require PowerShell 7 to run current script!"
        Write-Host "If already installed, run current script with pwsh command explicitly:"
        Write-Host "pwsh $PSCommandPath"
        return
    }
    install_vcpkg_packages
    build_3rdparty_spvgen
}

setup_radcpp

Pop-Location
