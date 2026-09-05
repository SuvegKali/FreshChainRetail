g++ -std=c++17 -I include src/*.cpp -o FreshChainRetail.exe
if ($?) {
    Write-Host "Build Successful. Running FreshChainRetail.exe..." -ForegroundColor Green
    .\FreshChainRetail.exe
} else {
    Write-Host "Build Failed." -ForegroundColor Red
}