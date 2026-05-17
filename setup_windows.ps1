# setup_windows.ps1
Write-Host "----------------------------------------------------------------------------"
Write-Host " Automatically set up development environment for Windows-platform"
Write-Host "----------------------------------------------------------------------------"
Write-Host ""
Write-Host " Includes 64bit-datatypes, float-datatypes, Ethernet-Interface,"
Write-Host " ASN1-encoding, ..."
Write-Host ""
Write-Host " Using Boost from .\external\boost.1.87.0"
Write-Host "----------------------------------------------------------------------------"

$forte_bin_dir = "bin/windows"

#set to boost-include directory
$forte_boost_test_inc_dirs = "$(Get-Location)\external\boost.1.87.0\lib\native\include"
#set to boost-library directory
$forte_boost_test_lib_dirs = "$(Get-Location)\external\boost.1.87.0\lib\native\include"

if (!(Test-Path $forte_bin_dir)) {
    New-Item -ItemType Directory -Path $forte_bin_dir | Out-Null
}

if (Test-Path $forte_bin_dir) {

    Write-Host "For building forte go to $forte_bin_dir and execute 'ninja'"
    Write-Host "forte can be found at $forte_bin_dir/src"
    Write-Host "forte_tests can be found at $forte_bin_dir/tests"


    Set-Location $forte_bin_dir

    cmake -G "Ninja" `
        -DFORTE_ARCHITECTURE=Win32 `
        -DFORTE_COM_ETH=ON `
        -DFORTE_COM_FBDK=ON `
        -DFORTE_COM_LOCAL=ON `
        -DFORTE_TESTS=ON `
        "-DFORTE_TESTS_INC_DIRS=$forte_boost_test_inc_dirs" `
        "-DFORTE_TESTS_LINK_DIRS=$forte_boost_test_inc_dirs" `
        -DFORTE_MODULE_CONVERT=ON `
        -DFORTE_MODULE_IEC61131=ON `
        -DFORTE_MODULE_UTILS=ON `
        ../../
    
    Set-Location ../../
} else {
    Write-Error "Unable to create $forte_bin_dir"
    exit 1
}
