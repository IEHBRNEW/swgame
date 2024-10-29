if (-not (Get-Command docker -ErrorAction SilentlyContinue)) {
    Write-Host "please install docker" -ForegroundColor Red
    exit
}

$imageName = "swgame"

Write-Host "build docker '$imageName'..."
docker build -t $imageName .

if ($LASTEXITCODE -ne 0) {
    Write-Host "error building docker image" -ForegroundColor Red
    exit
}

Write-Host "run docker image '$imageName'..."
docker run --rm $imageName

if ($LASTEXITCODE -ne 0) {
    Write-Host "error running docker image" -ForegroundColor Red
    exit
}

Read-Host -Prompt "press any button..."