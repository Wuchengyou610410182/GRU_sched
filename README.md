# GRU_sched
GRU_sched is a dynamic Linux CPU scheduler using a GRU prediction model and eBPF via SCHED_EXT. It improves responsiveness, reduces context switches, and lowers CPU migration. Compared to CFS and EEVDF, it shows better throughput, latency control, and runtime efficiency across CPU-, mixed-, and I/O-bound workloads, despite rare latency spikes.


This research explores the optimization of Linux CPU scheduling using machine learn-
ing (ML). We design a dynamic CPU scheduling policy incorporating a Gated Recurrent Unit
(GRU) prediction model and use eBPF for real-time monitoring. Leveraging SCHED_EXT, our
approach provides an adaptive scheduling mechanism for varying workloads. Comparative anal-
ysis against CFS and EEVDF shows that our GRU-based scheduler reduces context switching,
improves system responsiveness, and lowers CPU migration overhead. The GRU_sched sched-
uler consistently outperforms the Default Linux scheduler across CPU-bound, mixed-bound, and
I/O-bound workloads in terms of responsiveness, task completion, and latency management.
In CPU-bound workloads, GRU_sched shows a slight throughput increase and reduced
latency compared to the Default scheduler. The median throughput for GRU_sched in single-
threaded tests was 4288 events/sec, with maximum throughput reaching 4517 events/sec, com-
pared to the Default scheduler＇s 4287 events/sec and 4528 events/sec. Despite higher maximum
latencies (up to 65 ms) in some cases, GRU_sched reduced task runtime by 15.5%, improving
efficiency and handling tail latencies 25.8% better. In mixed-bound workloads, GRU_sched
outperforms the Default scheduler in throughput (16,453 events/sec vs. 16,429 events/sec) and
shows minimal jitter in average latency (0.49 ms median). However, it occasionally experi-
ences higher maximum latency spikes (up to 209.34 ms compared to 114.85 ms for Default),
but remains more responsive overall in dynamic environments.
In I/O-bound workloads, GRU_sched reduces context switching overhead with lower con-
text switch counts (below 130 compared to Default＇s 290). This leads to a 16.7% reduction
in total runtime and a 19.9% decrease in task processing, while maintaining stable through-
put and low average latency (0.23–0.24 ms). GRU_sched excelled at reducing latencies for
key processes such as snapd, kworker, and khungtaskd, improving system responsiveness. In
conclusion, GRU_Sched offers clear advantages in CPU-bound, mixed-bound, and I/O-bound
workloads, reducing task completion time, managing latency, and improving system-wide effi-
ciency. Despite occasional high-latency spikes, GRU_sched＇s overall performance improve-
ments make it an excellent choice for environments where performance and responsiveness are
crucial, highlighting its potential as a robust scheduler for various workloads.
Keywords: Gated Recurrent Unit (GRU), Open Neural Network Exchange, Linux Kernel
Scheduler, Extended Berkeley Packet Filter (ebpf)
