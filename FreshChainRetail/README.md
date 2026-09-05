# FreshChain Retail - Perishable Inventory & Markdown Engine

This is a C++ command-line application designed for FreshChain Retail to handle Perishable Inventory Allocation & Markdown management. 
It closely follows the design specified in the provided UML class diagram.

## How to Build and Run (Windows)

We provide a convenient PowerShell build script (`build.ps1`) to compile and run the project automatically.

1. Open PowerShell in this folder (`C:\Users\YASH VEER SINGH\Desktop\hacker\FreshChainRetail`).
2. Run the build script:
   ```powershell
   .\build.ps1
   ```
*(Requires `g++` to be installed and available in your PATH).*

## Design Patterns Used

1. **Strategy Pattern** is applied extensively for high cohesion and decoupling:
   - `AllocationStrategy` & `DemandBasedAllocationStrategy`: Determines the exact capacity calculations mapping inventory to stores.
   - `MarkdownStrategy` & `StandardMarkdownStrategy`: Evaluates dynamic markdown rules based on custom criteria mapped per category (days-to-expiry thresholds).

## Business-Rule Assumptions & Edge Case Handling

1. **Unallocatable Batch Remainder:** Explicitly handled inside `DemandBasedAllocationStrategy::allocate`. If `quantity` exceeds the total possible throughput capability of all stores, a `Warning` log is natively printed and the remaining stock sits idle at the DC.
2. **Selling more than capacity:** Explicitly handled in `Store::recordSale()`. Logs a `std::cerr` error if you try to push more volume than physically remains in FEFO-sorted viable stock.
3. **FEFO Inventory processing:** Standard library `std::sort` correctly prioritizes ascending `expiryDate` boundaries during sales transactions.
4. **Zero Demand Stores:** Handled gracefully; capacity is computed as `0`, effectively skipping the distribution loop cleanly without arithmetic exception overhead.
5. **Expired Intake Check:** Expiry evaluation is tested directly on the incoming loop during `DistributionCenter::receiveBatch()`.

## Note on Extra Code File
A file named `AliceBobHelper.cpp` is included in the `src` folder. This handles the specific snippet provided in the request while keeping the LLD completely intact.

