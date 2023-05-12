Push-Location $PSScriptRoot
Write-Host "Current working directory: $(Get-Location)"

$vcpkgRoot = $env:VCPKG_ROOT
if ($null -eq $vcpkgRoot -or !(Test-Path $vcpkgRoot -PathType Container)) {
    Write-Host "You need to setup vcpkg and set `$env:VCPKG_ROOT to its path."
    Write-Host "Please refer to: https://vcpkg.io/en/getting-started.html"
    return
}

function generate_project_files {
    Write-Host "Generating project files ..."
    Set-Location $PSScriptRoot
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

generate_project_files

Pop-Location
