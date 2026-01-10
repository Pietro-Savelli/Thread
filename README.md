
# Thread – Programmazione Concorrente in C

Repository contenente esercizi pratici sulla programmazione concorrente con thread in C, sviluppati per la preparazione all'esame universitario.

## Descrizione

Questa repository raccoglie una serie di esercizi implementati in C che coprono i concetti fondamentali della programmazione multi-thread, con particolare attenzione a problemi di sincronizzazione e comunicazione tra processi concorrenti.

## Struttura del Progetto

La repository è organizzata in cartelle tematiche, ciascuna contenente un esercizio specifico:

### Esercizi Disponibili

- **Esercizio buffer con statistiche live**  
  Implementazione di un buffer condiviso con monitoraggio in tempo reale delle statistiche di produzione e consumo.

- **Esercizio limite-produzione**  
  Gestione di vincoli sulla produzione con controllo dei limiti di capacità e sincronizzazione tra produttori.

- **Esercizio pila-ciclica**  
  Implementazione di una struttura dati pila ciclica thread-safe con gestione dell'overflow e underflow.

- **Esercizio produttore-consumatore base**  
  Implementazione classica del problema produttore-consumatore con buffer limitato.

- **Esercizio produttori con ID Sequenziali**  
  Estensione del problema produttore-consumatore con gestione di identificatori sequenziali per gli elementi prodotti.

- **Simulatore Scheduler**  
  Simulazione di uno scheduler per la gestione di processi concorrenti con diverse politiche di scheduling.

## Prerequisiti

- Compilatore GCC o Clang
- Sistema operativo POSIX-compliant (Linux, macOS, BSD)
- Libreria pthread
- Make (opzionale, per la compilazione automatizzata)

## Compilazione

Per compilare un singolo esercizio:

```bash
cd "nome-cartella-esercizio"
gcc -pthread -o programma *.c
```

Per eseguire il programma compilato:

```bash
./programma
```

## Concetti Implementati

Gli esercizi coprono i seguenti argomenti della programmazione concorrente:

- **Creazione e gestione di thread** con pthread
- **Mutex e lock** per la sincronizzazione
- **Variabili di condizione** per la comunicazione tra thread
- **Problema produttore-consumatore** in diverse varianti
- **Buffer circolari** e strutture dati thread-safe
- **Gestione delle race condition** e prevenzione dei deadlock
- **Scheduling** e gestione della concorrenza

## Note di Implementazione

Gli esercizi utilizzano la libreria POSIX Threads (pthread) e implementano pattern comuni della programmazione concorrente. Particolare attenzione è stata posta su:

- Corretta sincronizzazione tra thread
- Prevenzione di race condition
- Gestione efficiente delle risorse condivise
- Evitare deadlock e starvation
- Output leggibile per il debug e la comprensione del comportamento

## Stato del progetto
Progetto in evoluzione: nuovi esercizi possono essere aggiunti per approfondire ulteriori pattern di concorrenza.

## Scopo
Questo progetto ha finalità esclusivamente didattiche ed è pensato per l’esercitazione sui thread e sui meccanismi di sincronizzazione in C, in preparazione a esami universitari.
