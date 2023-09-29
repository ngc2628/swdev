# $ErrorActionPreference = ('Continue' (default), 'SilentlyContinue', 'Inquire', and 'Stop')
$ErrorActionPreference = "SilentlyContinue" 
# Get-ChildItem -recurse | Get-Content | Select-String -pattern "include"

$rootdir
$namelikeL = @()
$namelikeL += "*"
$cmdoptL = @("-h","-dir")

function chkarg([string]$arg) {
  foreach ($ii in $cmdoptL) {
    if ($arg.CompareTo($ii) -eq 0) {
      return $arg
    }
  }
  return $null
}

if ($args.count -eq 0) {
  write-host "usage : du.ps1 [-h] [-help] [--help] [-dir] root-dir"
  write-host "h | help: these lines"
  write-host "dir : root directory for searching - defaults to current working dir"
  write-host
}

$chkopt
$tmp
for ($ii=0; $ii -lt $args.count; $ii++) { 
  $tmp=chkarg($args[$ii])
  if ($tmp.CompareTo("-h") -eq 0 -or $tmp.CompareTo("-help") -eq 0 -or $tmp.CompareTo("--help") -eq 0) {
    write-host "usage : psgrep.ps1 [-h] [-help] [--help] [-dir] root-dir"
    write-host "h | help: these lines"
    write-host "dir : root directory for searching - defaults to current working dir"
    exit 0
  }
  if ($chkopt) {
    if ($tmp) {
      $chkopt=$tmp
      continue
    }
    if ($chkopt.CompareTo("-dir") -eq 0) {
      $rootdir = $args[$ii]
    }
  }
  $chkopt=$tmp
}

write-host "checking " $rootdir
$fileL = Get-ChildItem $rootdir 
write-host
write-host "found " $fileL.count " items"
write-host
$resL=@{}
for ($ii=0; $ii -lt $fileL.count; $ii++) { 
  $sz=Get-ChildItem $fileL[$ii] -recurse | Measure-Object -property length -sum
  $resL.Add($fileL[$ii], $sz.Sum)
}
foreach($item in $resL.GetEnumerator() | Sort Value)
{
  $mbsz=$item.Value/1000000
  write-host $item.Name " : " $mbsz " MB"
}

