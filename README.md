# GRU_sched
GRU_sched is a dynamic Linux CPU scheduler using a GRU prediction model and eBPF via SCHED_EXT. It improves responsiveness, reduces context switches, and lowers CPU migration. Compared to CFS and EEVDF, it shows better throughput, latency control, and runtime efficiency across CPU-, mixed-, and I/O-bound workloads, despite rare latency spikes.
