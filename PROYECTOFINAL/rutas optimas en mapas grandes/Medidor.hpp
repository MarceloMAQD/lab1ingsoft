#ifndef MEDIDOR_HPP
#define MEDIDOR_HPP

#include <iostream>
#include <chrono>

#ifdef _WIN32
#include <windows.h>
#include <psapi.h>
#else
#include <sys/resource.h>
#endif

size_t getMemoryUsageKB() {
#ifdef _WIN32
    PROCESS_MEMORY_COUNTERS pmc;
    GetProcessMemoryInfo(GetCurrentProcess(), &pmc, sizeof(pmc));
    return pmc.WorkingSetSize / 1024; // En KB
#else
    struct rusage usage;
    getrusage(RUSAGE_SELF, &usage);
    return usage.ru_maxrss; // En KB
#endif
}

template<typename Func>
void medirAlgoritmo(const std::string& nombre, Func algoritmo) {
    // --- Medición de memoria inicial ---
    size_t memoriaInicioKB = getMemoryUsageKB();

    // --- Medición de tiempo ---
    auto start = std::chrono::high_resolution_clock::now();

    // Ejecutar el algoritmo
    algoritmo();

    // --- Medición de tiempo final ---
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> duracion = end - start;

    // --- Medición de memoria final ---
    size_t memoriaFinKB = getMemoryUsageKB();
    size_t memoriaUsadaKB = (memoriaFinKB > memoriaInicioKB) ? (memoriaFinKB - memoriaInicioKB) : 0;

    // --- Convertir a bytes ---
    size_t memoriaUsadaBytes = memoriaUsadaKB * 1024;

    // --- Obtener pico de memoria ---
#ifdef _WIN32
    PROCESS_MEMORY_COUNTERS_EX pmc;
    GetProcessMemoryInfo(GetCurrentProcess(), (PROCESS_MEMORY_COUNTERS*)&pmc, sizeof(pmc));
    size_t picoMemoriaBytes = pmc.PeakWorkingSetSize; // Bytes
#else
    struct rusage usage;
    getrusage(RUSAGE_SELF, &usage);
    size_t picoMemoriaBytes = usage.ru_maxrss * 1024; // Convertir KB a bytes
#endif

    // --- Mostrar resultados ---
    std::cout << nombre << ":\n";
    std::cout << "  Tiempo: " << duracion.count() << " ms\n";
    std::cout << "  Memoria usada: " << memoriaUsadaBytes << " bytes (" << memoriaUsadaKB << " KB)\n";
    std::cout << "  Pico de memoria: " << picoMemoriaBytes << " bytes\n\n";
}

#endif
