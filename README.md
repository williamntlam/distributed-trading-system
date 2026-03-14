# Distributed Trading System (DTS)

A high-performance, low-latency electronic trading platform designed
with a hybrid architecture. DTS leverages Go for a high-concurrency gRPC
gateway and Modern C++ (C++20) for a deterministic, lock-free matching
engine.

## Architecture Overview

DTS follows a separation-of-concerns architecture:

-   Ingress Gateway (Go): Handles thousands of concurrent gRPC client
    connections, performs pre-trade risk checks, and validates order
    integrity.
-   Matching Engine (C++20): A single-threaded, cache-optimized core
    that manages the Central Limit Order Book (CLOB) with price-time
    priority.
-   IPC Layer: Ultra-low latency communication via shared memory (mmap)
    and an SPSC (single-producer, single-consumer) lock-free ring
    buffer.

## High-Level Architecture

``` mermaid
flowchart LR
    Clients[Trading Clients / Algos] -->|gRPC Streams| Gateway[Go Gateway]
    Gateway --> Risk[Risk Validation]
    Risk --> IPC[SPSC Ring Buffer / Shared Memory]
    IPC --> Engine[C++20 Matching Engine]
    Engine --> Book[Central Limit Order Book]
    Engine --> Events[Execution Events]
    Events --> Publisher[Market Data Publisher]
    Publisher --> Clients
```

## Order Processing Sequence

``` mermaid
sequenceDiagram
    participant Client
    participant Gateway
    participant Risk
    participant IPC
    participant Engine
    participant OrderBook
    participant Audit

    Client->>Gateway: Submit Order
    Gateway->>Risk: Validate order
    Risk-->>Gateway: Approved
    Gateway->>Audit: Log instruction timestamp
    Gateway->>IPC: Write instruction
    IPC->>Engine: Deliver message
    Engine->>OrderBook: Apply matching logic
    OrderBook-->>Engine: Fill / rest / reject
    Engine->>Audit: Log resulting event
    Engine-->>Gateway: Execution event
    Gateway-->>Client: Order update
```

## Low-Latency Data Path

``` mermaid
flowchart TD
    A[Client Order]
    B[gRPC Ingress]
    C[Schema Validation]
    D[Risk Checks]
    E[Binary Encoding]
    F[Shared Memory Write]
    G[Ring Buffer Read]
    H[Matching Logic]
    I[Order Book Update]
    J[Execution Event]
    K[Market Data / Client Ack]

    A --> B --> C --> D --> E --> F --> G --> H --> I --> J --> K
```

## Project Structure

``` text
/distributed-trading-system
├── api
│   └── proto/v1
│       ├── trading.proto
│       └── market.proto
├── services
│   ├── gateway
│   └── engine
├── internal
│   ├── protocol
│   └── shm
├── scripts
└── docker-compose.yml
```

## Tech Stack

-   Go: High concurrency networking layer.
-   C++20: Deterministic matching engine and memory control.
-   gRPC: Bidirectional order entry streaming.
-   Shared Memory (mmap): Zero-copy IPC.
-   Protobuf v3: Language-agnostic message serialization.

## Performance Design Principles

Zero-Allocation Hot Path\
The C++ engine uses preallocated object pools and intrusive structures
to avoid heap fragmentation and latency jitter.

Lock-Free IPC\
Atomic memory barriers (acquire/release) synchronize the gateway and
engine without OS mutexes.

Cache-Line Alignment\
Critical structures use alignas(64) to avoid false sharing.

Fixed-Point Math\
Prices use int64 fixed-point arithmetic to eliminate floating point
rounding issues.

## Getting Started

### Prerequisites

-   Go 1.21+
-   Clang 15+ or GCC 12+
-   Protobuf compiler (protoc)
-   Linux environment (required for mmap and shm_open)

### Build

``` bash
make proto

cd services/gateway
go build -o gateway

cd ../engine
mkdir build
cd build
cmake ..
make
```

## Deterministic Traceability

Every instruction received by the gateway is timestamped and mapped to
resulting events produced by the engine. This enables full
reconstruction of market states for auditing and debugging.

## License

This project is intended for educational and portfolio purposes.
