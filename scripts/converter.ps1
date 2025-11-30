# scripts/converter.ps1 - Converte saída para visualizador de filósofos
param(
    [string]$InputFile = "",
    [string]$OutputFile = "visualizer_output.txt"
)

function Convert-BancoOutput {
    param([string]$text)
    
    $converted = $text -replace "(\d+) CONSULTOR (\d+) consultando saldo", '$1 $2 is eating' `
                      -replace "(\d+) CONSULTOR (\d+) realizando transferencia", '$1 $2 is eating' `
                      -replace "(\d+) CONSULTOR (\d+) analisando extrato", '$1 $2 is thinking' `
                      -replace "(\d+) CONSULTOR (\d+) aguardando acesso", '$1 $2 is sleeping' `
                      -replace "(\d+) CONSULTOR (\d+) conta bloqueada", '$1 $2 died' `
                      -replace "(\d+) OPERADOR (\d+) consultando saldo", '$1 $2 is eating' `
                      -replace "(\d+) OPERADOR (\d+) realizando transferencia", '$1 $2 is eating' `
                      -replace "(\d+) OPERADOR (\d+) analisando extrato", '$1 $2 is thinking' `
                      -replace "(\d+) OPERADOR (\d+) aguardando acesso", '$1 $2 is sleeping' `
                      -replace "(\d+) OPERADOR (\d+) conta bloqueada", '$1 $2 died'
    
    return $converted
}

Write-Host " CONVERSOR PARA VISUALIZADOR" -ForegroundColor Cyan
Write-Host "=================================" -ForegroundColor Cyan

if ($InputFile -ne "" -and (Test-Path $InputFile)) {
    Write-Host " Convertendo arquivo: $InputFile" -ForegroundColor Yellow
    $inputContent = Get-Content $InputFile -Raw
    $convertedContent = Convert-BancoOutput -text $inputContent
    $convertedContent | Set-Content $OutputFile
    Write-Host " Arquivo convertido: $OutputFile" -ForegroundColor Green
    
    Write-Host "`n PRÓXIMOS PASSOS:" -ForegroundColor White
    Write-Host "1. Acesse: https://nafuka11.github.io/philosophers-visualizer/" -ForegroundColor Cyan
    Write-Host "2. Cole o conteúdo de: $OutputFile" -ForegroundColor Cyan
    Write-Host "3. Configure os parâmetros:" -ForegroundColor Cyan
    Write-Host "   - Number of philosophers: 5" -ForegroundColor White
    Write-Host "   - Time to die: 410" -ForegroundColor White
    Write-Host "   - Time to eat: 200" -ForegroundColor White
    Write-Host "   - Time to sleep: 200" -ForegroundColor White
} else {
    Write-Host " Arquivo não encontrado: $InputFile" -ForegroundColor Red
    Write-Host "`n COMO USAR:" -ForegroundColor White
    Write-Host ".\scripts\converter.ps1 -InputFile 'saida.txt'" -ForegroundColor Yellow
    Write-Host "OU arraste um arquivo .txt para converter.bat" -ForegroundColor Yellow
}