if (-not (Get-Command docker -ErrorAction SilentlyContinue)) {
    Write-Host "please install docker" -ForegroundColor Red
    exit
}

$imageName = "swgame"

$folderPath = "./test/data/"
$fullFolderPath = Resolve-Path $folderPath

Get-ChildItem -Path $fullFolderPath -File -Recurse | ForEach-Object {
    $sample = $_.FullName.Replace("$fullFolderPath", "").TrimStart('\')
	
	$sample = $sample -replace '\\', '/'
	
    Write-Host "`nRunning Docker image '$imageName' with sample '$sample'..."

    docker run -e SAMPLE_NAME=$sample --rm $imageName
}

Read-Host -Prompt "press any button..."