set terminal png size 600,400
set xlabel 'Fractional Bits'
set ylabel 'Error'
set output './result/PrecisionLoss.png'
plot    './data/ErrorVsFractionBits.txt' using 1:2 with lines title 'Fixed Error Vs Precision On Matrix Size 4'
set output './result/MultimeVsSize.png'
set xlabel 'MatrixSize'
set ylabel 'Time (in s)'
plot    './data/TimeVsSize.txt' using 1:2 with lines title 'Fixed', \
        './data/TimeVsSize.txt' using 1:3 with lines title 'Float
