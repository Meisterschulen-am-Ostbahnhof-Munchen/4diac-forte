# make_windows.ps1
Write-Host "----------------------------------------------------------------------------"

$forte_bin_dir = "bin/windows"

# Remove old folder if exists
if (Test-Path $forte_bin_dir) {
    Remove-Item -Recurse -Force $forte_bin_dir
}

# Call setup_windows.ps1
& .\setup_windows.ps1

# Goto new Directory
if (Test-Path $forte_bin_dir) {
    Set-Location $forte_bin_dir

    # Start build
    ninja -j $env:NUMBER_OF_PROCESSORS

    Write-Host "----------------------------------------------------------------------------"
    Write-Host " DONE. To re-build just type:"
    Write-Host " cd $forte_bin_dir"
    Write-Host " ninja"
    Write-Host "----------------------------------------------------------------------------"
    
    Set-Location ../../
} else {
    Write-Error "Build directory was not created."
}
