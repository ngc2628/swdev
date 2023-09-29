# Set-ExecutionPolicy Unrestricted
# $ErrorActionPreference = ('Continue' (default), 'SilentlyContinue', 'Inquire', and 'Stop')
$ErrorActionPreference = "SilentlyContinue" 
# Get-ChildItem -recurse | Get-Content | Select-String -pattern "include"

$rootdir
$namelikeL = @()
$justsum=0
$cmdoptL = @("-h","-help","--help","-dir","-s")

function chkarg([string]$arg) {
  foreach ($ii in $cmdoptL) {
    if ($arg.CompareTo($ii) -eq 0) {
      return $arg
    }
  }
  return $null
}

$global:sz=0

function szobj($ff) {
  $info=Measure-Object -property length -sum -InputObject $ff
  $global:sz=$global:sz+$info.sum
}

function chkrec($ff) {
  if ($ff.PSIsContainer) {
    $fileL=Get-ChildItem $ff.FullName
    for ($ii=0; $ii -lt $fileL.count; $ii++) {
      if ($fileL[$ii].PSIsContainer) {
        chkrec($fileL[$ii])
      }
      else {
        szobj($fileL[$ii])
      }
    }
  }
  else {
    szobj($ff)
  }
}

$chkopt
$tmp
for ($ii=0; $ii -lt $args.count; $ii++) { 
  $tmp=chkarg($args[$ii])
  if ($tmp.CompareTo("-h") -eq 0 -or $tmp.CompareTo("-help") -eq 0 -or $tmp.CompareTo("--help") -eq 0) {
    write-host ""
    write-host "usage : du.ps1 [-h | -help | --help] [-dir] root-dir [-s]"
    write-host "h | help : these lines"
    write-host "dir : root directory for searching - defaults to current working dir"
    write-host "s : total sum"
    exit 0
  }
  if ($tmp.CompareTo("-s") -eq 0) {
    $justsum=1
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

$fileL = Get-ChildItem $rootdir
$tmpL=@()
$info
$ftype
for ($ii=0; $ii -lt $fileL.count; $ii++) {
  if ($fileL[$ii].PSIsContainer) {
    $ftype="[d]"
  }
  else {
    $ftype="[f]"
  }
  chkrec($fileL[$ii])
  $global:sz=$global:sz/1000
  $fmt = $global:sz
  write-host $ftype " " $fmt " kb " $fileL[$ii].FullName
  $global:sz=0
}


