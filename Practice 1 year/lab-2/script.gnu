set terminal png
set output "graph.png"

set xrange [2:8]

set yrange [0:10]

set grid

set ytics 1

set xtics 2

set ylabel "Speedup"

set xlabel "Number of threads"

plot "data.txt" u 1:2 with linespoints lw 2 pt 3 ps 1 title "N = 15000", "data.txt" u 1:3 with linespoints lw 2 pt 3 ps 1 title "N = 20000", "data.txt" u 1:4 with linespoints lw 2 pt 3 ps 1 title "N = 25000", "data.txt" u 1:5 with linespoints lw 2 pt 3 ps 1 title "Linear speedup"
