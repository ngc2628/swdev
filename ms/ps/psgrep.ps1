# $ErrorActionPreference = ('Continue' (default), 'SilentlyContinue', 'Inquire', and 'Stop')
$ErrorActionPreference = "SilentlyContinue" 
# Get-ChildItem -recurse | Get-Content | Select-String -pattern "include"

$rootdir
$namelikeL = @()
$match
$cs=1
$cmdoptL = @("-h","-help","--help","-dir","-name","-match","-imatch")

function chkarg([string]$arg) {
  foreach ($ii in $cmdoptL) {
    if ($arg.CompareTo($ii) -eq 0) {
      return $arg
    }
  }
  return $null
}

function usage() {
  write-host
  write-host "usage : psgrep.ps1 [-h|-help] [-dir root-dir] [-name file-pattern] [-(i)match content-string]"
  write-host
  write-host "  -h | -help: these lines"
  write-host "  -dir : root directory for searching - defaults to current working dir"
  write-host "  -name : file filter (e.g. *.txt), may be given several times - defaults to *"
  write-host "  -(i)match : refrain to files and lines containing givin string (i:case insensitive) - ignored if empty"
  write-host
  return $null
}

if ($args.count -eq 0) {
  usage
#  write-host "therefore no-args means find all files from current working dir ..."
  exit 0
}

$chkopt
$tmp
for ($ii=0; $ii -lt $args.count; $ii++) { 
  $tmp=chkarg($args[$ii])
  if ($tmp.CompareTo("-h") -eq 0 -or $tmp.CompareTo("-help") -eq 0 -or $tmp.CompareTo("--help") -eq 0) {
    usage
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
    elseif ($chkopt.CompareTo("-name") -eq 0) {
      $namelikeL += $args[$ii]
    }
    elseif ($chkopt.CompareTo("-match") -eq 0) {
      $match = $args[$ii]
    }
    elseif ($chkopt.CompareTo("-imatch") -eq 0) {
      $match = $args[$ii]
      $cs=0
    }
  }
  $chkopt=$tmp
}

if ($namelikeL.count -eq 0) {
  $namelikeL += "*"
}

$fileL = Get-ChildItem $rootdir -Force -recurse -include $namelikeL
write-host
write-host "found " $fileL.count " files"
write-host
for ($ii=0; $ii -lt $fileL.count; $ii++) { 
  $mod=(Get-Item $fileL[$ii]).LastWriteTime.ToString("yy-MM-ddTHH:mm:ss")
  if ($match) {
    $found=0
    $linenr=0
    $matching=0
    $chkline=0
    $content = Get-Content $fileL[$ii]
    foreach ($line in $content) {
      $linenr++
      if ($cs -eq 0) {
        $chkline=$line.ToLower().Contains($match.ToLower())
      }
      else {
        $chkline=$line.Contains($match)
      }
      if ($chkline) {
        $matching++
        if ($found -eq 0) {
          write-host "file : " $fileL[$ii] " " $mod
        }
        write-host "line " $linenr " : " $line
        $found=1
      }
    }
    if ($found -eq 1) {
      write-host "found " $matching " matching lines"
      write-host
    }
  }
  else {
    write-host "file : " $mod " " $fileL[$ii]
  }
}