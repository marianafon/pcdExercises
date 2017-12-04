#pragma omp parallel for num_threads(thread_count) \
    default(none) private(i) shared(a, n)
for (i = 0; i < n; i++) {
    a[i] = i * (i + 1) / 2;
}