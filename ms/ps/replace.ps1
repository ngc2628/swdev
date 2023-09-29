# powershell test
$ErrorActionPreference="SilentlyContinue" 

$cmdoptL=@("-h","-help","--help","-fin","-fout","-fsubs","-stage")

function chkarg([string]$arg) {
  foreach ($ii in $cmdoptL) {
    if ($arg.compareto($ii) -eq 0) {
      return $arg
    }
  }
  return $null
}

function usage() {
  write-host
  write-host "usage : $PSCommandPath"
  write-host
  write-host "  -h | -help: these lines"
  write-host "  -fin: file input"
  write-host "  -fout: file output"
  write-host "  -fsubs: file with substitution entries (xml)"
  write-host "  -stage: test|entw|schl"
  write-host
  return $null
}

$fin=$null
$fout=$null
$fsubs=$null
$stagename=$null

# command line arguments
$chkopt
$tmp
for ($ii=0;$ii -lt $args.count;$ii++) { 
  $tmp=chkarg($args[$ii])
  if ($tmp.compareto("-h") -eq 0 -or $tmp.compareto("-help") -eq 0 -or $tmp.compareto("--help") -eq 0) {
    usage
    exit 0
  }
  if ($chkopt) {
    if ($tmp) {
      $chkopt=$tmp
      continue
    }
    if ($chkopt.compareto("-fin") -eq 0) {
      $fin=$args[$ii]
    }
    elseif ($chkopt.compareto("-fout") -eq 0) {
      $fout=$args[$ii]
    }
    elseif ($chkopt.compareto("-fsubs") -eq 0) {
      $fsubs=$args[$ii]
    }
    elseif ($chkopt.compareto("-stage") -eq 0) {
      $stagename=$args[$ii]
    }
  }
  $chkopt=$tmp
}
# command line arguments

if ($fin -eq $null -or $fin -eq "" -or $fout -eq $null -or $fout -eq "" -or
    $fsubs -eq $null -or $fsubs -eq "" -or $stagename -eq $null -or $stagename -eq "") {
  usage
  exit 1
}

# file test
if (-not $fin) {
  usage
  exit 1
}
if (-not $fsubs) {
  usage
  exit 1
}
# file test

# static names
# comment lines starting "//"
$enciso88591=[System.Text.Encoding]::GetEncoding('iso-8859-1')
$encutf8=[System.Text.Encoding]::GetEncoding('utf-8')
# $encutf8.getstring([System.Text.Encoding]::Convert($enciso88591,$encutf8,$enciso88591.GetBytes($txt)))
$kvp_sep=":="
$hausspezname="hausspez"
$replname="replacements"
$amname="amendments"
# static names

[xml]$xmltree=get-content -path $fsubs
$replnode
foreach ($entry in $xmltree.$hausspezname.childnodes) {
  if ($entry.name.compareto($replname) -eq 0) {
    $replnode=$entry
    break
  }
}

if ($replnode -eq $null) {
  usage 
  exit 1
}
$subsL=@()
foreach ($entry in $replnode.childnodes) {
  $subsL+=$entry
}
if ($subsL.count -eq 0) {
  usage
  exit 1
}

$wcontent=@()
$wline
$kvp=@()
$content=get-content -encoding iso-8859-1 $fin

foreach ($line in $content) {
  $wline=$line 
  if ( ($line -match("^\s*//")) -or ($line -match("^\s*$")) ) { 
    $wcontent+=$wline
    continue
  } 
  $kvp=$line.split($kvp_sep)
  :look foreach ($entry in $subsL) {
    if ($kvp[0].trim().compareto($entry.name) -eq 0) {
      foreach ($stage in $entry.childnodes) {
        if ($stage.name.tolower().compareto($stagename.tolower()) -eq 0) {
           if ($stage.innertext -ne $null -and $stage.innertext -ne "") {
             $wline=$kvp[0]+$kvp_sep+" "+$stage.innertext
           }
           else {
             $wline=$null
           }
           break look  
        }
      }
    }
  }
  if ($wline -ne $null -and $wline -ne "") {
    $wcontent+=$wline
  }
}

out-file -encoding iso-8859-1 -filepath $fout -inputobject $wcontent

exit 0

