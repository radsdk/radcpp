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

function generate_project_files {
    Write-Host "Generating project files ..."
    $sourceDir = $PSScriptRoot
    $buildDir = Join-Path -Path $sourceDir -ChildPath "build/"
    $vcpkgToolchainFile = Join-Path -Path $vcpkgRoot -ChildPath "scripts/buildsystems/vcpkg.cmake"
    if (Test-Path -Path $buildDir -PathType Container) {
        Write-Host "Build directory already existed: $buildDir"
        while ($true) {
            $choice = Read-Host -Prompt "Remove exist build directory and make a clean build? Enter your choice (y/n for yes/no)"
            if ($choice -eq "y" -or $choice -eq "yes") {
                Remove-Item -Path $buildDir -Recurse -Force
                break
            }
            if ($choice -eq "n" -or $choice -eq "no") {
                break
            }
        }
    }
    if (!(Test-Path -Path $vcpkgToolchainFile -PathType Leaf)) {
        Write-Host "vcpkg toolchain file does not exist!"
        return
    }
    Write-Host "Build directory: $buildDir"
    Write-Host "vcpkg toolchain file: $vcpkgToolchainFile"
    Write-Host "cmake -S `"$sourceDir`" -B `"$buildDir`" -DCMAKE_TOOLCHAIN_FILE=`"$vcpkgToolchainFile`""
    Invoke-Expression "cmake -S `"$sourceDir`" -B `"$buildDir`" -DCMAKE_TOOLCHAIN_FILE=`"$vcpkgToolchainFile`""
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
    generate_project_files
}

setup_radcpp

Pop-Location
