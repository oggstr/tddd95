f=->x{x>0?"0125986"[x%7]+f[x/7]:""}
$<.each{|l|puts f[l.to_i]}