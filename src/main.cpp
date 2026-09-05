#include <iostream>
#include <memory>
#include <string>
#include <limits>
#include <map>
#include "../include/Date.h"
#include "../include/StandardMarkdownStrategy.h"
#include "../include/ProductCategory.h"
#include "../include/Store.h"
#include "../include/StockBatch.h"
#include "../include/DemandBasedAllocationStrategy.h"
#include "../include/DistributionCenter.h"

using namespace std;

void clearInput() {
    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
}

void runDemo() {
    cout << "\n========================================================\n";
    cout << "   KICKSTARTING AUTOMATED DEMO (4 CATEGORIES)\n";
    cout << "========================================================\n\n";

    Date today("2023-10-01");
    cout << "[Step 1] Initializing Distribution Center\n";
    cout << "  -> System Date set to: " << today.toString() << "\n\n";

    // Setup Strategies
    cout << "[Step 2] Setting up Pricing Markdown Strategies\n";
    cout << "  -> Defining Dairy Rules: 25% off at 3 days, 50% off at 1 day\n";
    map<int, double> dairyRules = {{3, 0.25}, {1, 0.50}};
    auto dairyMarkdown = make_shared<StandardMarkdownStrategy>(dairyRules);

    cout << "  -> Defining Produce Rules: 30% off at 2 days\n";
    map<int, double> produceRules = {{2, 0.30}};
    auto produceMarkdown = make_shared<StandardMarkdownStrategy>(produceRules);

    cout << "  -> Defining Bakery Rules: 80% off at 1 day\n";
    map<int, double> bakeryRules = {{1, 0.80}};
    auto bakeryMarkdown = make_shared<StandardMarkdownStrategy>(bakeryRules);

    cout << "  -> Defining Meat Rules: 20% off at 2 days, 40% off at 1 day\n\n";
    map<int, double> meatRules = {{2, 0.20}, {1, 0.40}};
    auto meatMarkdown = make_shared<StandardMarkdownStrategy>(meatRules);

    // Setup Product Categories
    cout << "[Step 3] Creating Product Categories\n";
    
    cout << "  -> Adding category 'Dairy'\n";
    cout << "     - Shelf life: 10 days\n";
    cout << "     - Base Price: $5.0\n";
    auto dairy = make_shared<ProductCategory>("Dairy", 10, 5.0, dairyMarkdown);
    
    cout << "  -> Adding category 'Produce'\n";
    cout << "     - Shelf life: 7 days\n";
    cout << "     - Base Price: $3.0\n";
    auto produce = make_shared<ProductCategory>("Produce", 7, 3.0, produceMarkdown);
    
    cout << "  -> Adding category 'Bakery'\n";
    cout << "     - Shelf life: 4 days\n";
    cout << "     - Base Price: $4.0\n";
    auto bakery = make_shared<ProductCategory>("Bakery", 4, 4.0, bakeryMarkdown);

    cout << "  -> Adding category 'Meat'\n";
    cout << "     - Shelf life: 5 days\n";
    cout << "     - Base Price: $15.0\n\n";
    auto meat = make_shared<ProductCategory>("Meat", 5, 15.0, meatMarkdown);

    // Setup Stores
    cout << "[Step 4] Registering Stores with Custom Category Demand Rates\n";
    cout << "  -> Adding 'Downtown Market'\n";
    cout << "     - Demand: Dairy (10.0), Produce (20.0), Bakery (15.0), Meat (5.0)\n";
    auto store1 = make_shared<Store>(1, "Downtown Market");
    store1->setDemandRate("Dairy", 10.0);
    store1->setDemandRate("Produce", 20.0);
    store1->setDemandRate("Bakery", 15.0);
    store1->setDemandRate("Meat", 5.0);

    cout << "  -> Adding 'Suburban Market'\n";
    cout << "     - Demand: Dairy (5.0), Produce (0.0), Bakery (5.0), Meat (10.0)\n";
    auto store2 = make_shared<Store>(2, "Suburban Market");
    store2->setDemandRate("Dairy", 5.0);
    store2->setDemandRate("Produce", 0.0); // Zero demand example
    store2->setDemandRate("Bakery", 5.0);
    store2->setDemandRate("Meat", 10.0);

    auto allocStrategy = make_shared<DemandBasedAllocationStrategy>();
    DistributionCenter dc(allocStrategy);
    dc.addStore(store1);
    dc.addStore(store2);

    // Receive Batches
    cout << "\n[Step 5] Receiving Stock Batches at the Distribution Center\n";
    StockBatch b1(101, dairy, 100, Date("2023-10-01"), Date("2023-10-06"));
    StockBatch b2(102, produce, 50, Date("2023-10-01"), Date("2023-10-03"));
    StockBatch b3(103, bakery, 30, Date("2023-10-01"), Date("2023-10-02"));
    StockBatch b4(104, meat, 40, Date("2023-10-01"), Date("2023-10-04"));
    StockBatch b5(105, dairy, 20, Date("2023-10-01"), Date("2023-09-30")); // Expired

    cout << "  -> Receiving Batch 101 (Dairy, Qty 100, Expires 2023-10-06)\n";
    dc.receiveBatch(b1, today);
    cout << "  -> Receiving Batch 102 (Produce, Qty 50, Expires 2023-10-03)\n";
    dc.receiveBatch(b2, today);
    cout << "  -> Receiving Batch 103 (Bakery, Qty 30, Expires 2023-10-02)\n";
    dc.receiveBatch(b3, today);
    cout << "  -> Receiving Batch 104 (Meat, Qty 40, Expires 2023-10-04)\n";
    dc.receiveBatch(b4, today);
    cout << "  -> Receiving Batch 105 (Dairy, Qty 20, Expires 2023-09-30) - EXPECT REJECTION:\n     ";
    dc.receiveBatch(b5, today); 

    // Allocate Batches
    cout << "\n[Step 6] Allocating Batches based on store demand rates\n";
    cout << "  -> Allocating Batch 101 (Dairy)\n";
    dc.allocateBatch(101, today);
    cout << "  -> Allocating Batch 102 (Produce)\n";
    dc.allocateBatch(102, today);
    cout << "  -> Allocating Batch 103 (Bakery)\n";
    dc.allocateBatch(103, today);
    cout << "  -> Allocating Batch 104 (Meat)\n";
    dc.allocateBatch(104, today);

    // Query Stores
    cout << "\n[Step 7] Querying On-Shelf Inventory for Stores (Notice Markdown Prices)\n";
    dc.queryStoreInventory(1, today);
    dc.queryStoreInventory(2, today);

    // Sales & FEFO handling
    cout << "\n[Step 8] Simulating Customer Sales (Testing FEFO and Max Limits)\n";
    cout << "  -> 'Downtown Market' sells 10 Dairy items.\n";
    store1->recordSale("Dairy", 10, today);
    cout << "  -> 'Downtown Market' sells 5 Meat items.\n";
    store1->recordSale("Meat", 5, today);
    cout << "  -> 'Suburban Market' tries to sell 50 Bakery items (only holds 5) - EXPECT ERROR:\n     ";
    store2->recordSale("Bakery", 50, today);

    // Process Write-offs
    Date future("2023-10-07"); 
    cout << "\n[Step 9] Fast-forwarding time to " << future.toString() << " and processing Expiry Write-offs\n";
    dc.processWriteOffs(future);
    
    cout << "\n========================================================\n";
    cout << "   DEMO COMPLETE\n";
    cout << "========================================================\n\n";
}

void runInteractiveMode() {
    cout << "\n========================================================\n";
    cout << "   STARTING INTERACTIVE MODE\n";
    cout << "========================================================\n";
    cout << "Tip: We've pre-loaded 4 Categories (Dairy, Produce, Bakery, Meat) for you!\n";
    
    Date currentDate("2023-10-01");
    
    // Setup standard categories for ease of use
    map<int, double> dairyRules = {{3, 0.25}, {1, 0.50}};
    auto dairyMarkdown = make_shared<StandardMarkdownStrategy>(dairyRules);
    map<int, double> produceRules = {{2, 0.30}};
    auto produceMarkdown = make_shared<StandardMarkdownStrategy>(produceRules);
    map<int, double> bakeryRules = {{1, 0.80}};
    auto bakeryMarkdown = make_shared<StandardMarkdownStrategy>(bakeryRules);
    map<int, double> meatRules = {{2, 0.20}, {1, 0.40}};
    auto meatMarkdown = make_shared<StandardMarkdownStrategy>(meatRules);
    
    auto dairy = make_shared<ProductCategory>("Dairy", 10, 5.0, dairyMarkdown);
    auto produce = make_shared<ProductCategory>("Produce", 7, 3.0, produceMarkdown);
    auto bakery = make_shared<ProductCategory>("Bakery", 4, 4.0, bakeryMarkdown);
    auto meat = make_shared<ProductCategory>("Meat", 5, 15.0, meatMarkdown);
    
    auto allocStrategy = make_shared<DemandBasedAllocationStrategy>();
    DistributionCenter dc(allocStrategy);
    
    map<int, shared_ptr<Store>> storesMap;

    while (true) {
        cout << "\n--- Interactive Menu (Current Date: " << currentDate.toString() << ") ---\n";
        cout << "1. Advance / Change Current Date\n";
        cout << "2. Add a new Store\n";
        cout << "3. Receive a Stock Batch at Distribution Center\n";
        cout << "4. Allocate a Batch to Stores\n";
        cout << "5. Record a Sale at a Store\n";
        cout << "6. Query a Store's Inventory\n";
        cout << "7. Process Write-offs (for expired items)\n";
        cout << "8. Exit Interactive Mode\n";
        cout << "Select an option (1-8): ";
        
        int choice;
        if (!(cin >> choice)) {
            clearInput();
            cout << "Invalid input. Please enter a number.\n";
            continue;
        }
        
        if (choice == 1) {
            cout << "Enter new date (YYYY-MM-DD): ";
            string d; cin >> d;
            currentDate = Date(d);
            cout << "Date updated to " << currentDate.toString() << "\n";
        } 
        else if (choice == 2) {
            cout << "Enter Store ID (number): ";
            int id; 
            if (!(cin >> id)) { clearInput(); cout << "Invalid ID.\n"; continue; }
            cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Clear newline before getline
            
            cout << "Enter Store Name: ";
            string name; getline(cin, name);
            
            auto store = make_shared<Store>(id, name);
            
            cout << "Enter daily demand rate for Dairy: ";
            double rate; cin >> rate; store->setDemandRate("Dairy", rate);
            cout << "Enter daily demand rate for Produce: ";
            cin >> rate; store->setDemandRate("Produce", rate);
            cout << "Enter daily demand rate for Bakery: ";
            cin >> rate; store->setDemandRate("Bakery", rate);
            cout << "Enter daily demand rate for Meat: ";
            cin >> rate; store->setDemandRate("Meat", rate);
            
            dc.addStore(store);
            storesMap[id] = store;
            cout << "Store '" << name << "' added successfully!\n";
        }
        else if (choice == 3) {
            cout << "Enter Batch ID (number): ";
            int bId; cin >> bId;
            cout << "Categories:\n1. Dairy\n2. Produce\n3. Bakery\n4. Meat\nSelect Category (1-4): ";
            int catChoice; cin >> catChoice;
            shared_ptr<ProductCategory> cat = dairy;
            if (catChoice == 2) cat = produce;
            else if (catChoice == 3) cat = bakery;
            else if (catChoice == 4) cat = meat;
            
            cout << "Enter Quantity: ";
            int qty; cin >> qty;
            cout << "Enter Arrival Date (YYYY-MM-DD): ";
            string arrStr; cin >> arrStr;
            cout << "Enter Expiry Date (YYYY-MM-DD): ";
            string expStr; cin >> expStr;
            
            StockBatch batch(bId, cat, qty, Date(arrStr), Date(expStr));
            dc.receiveBatch(batch, currentDate);
            cout << "Batch intake processed.\n";
        }
        else if (choice == 4) {
            cout << "Enter Batch ID to allocate: ";
            int bId; cin >> bId;
            dc.allocateBatch(bId, currentDate);
            cout << "Allocation complete.\n";
        }
        else if (choice == 5) {
            cout << "Enter Store ID: ";
            int sId; cin >> sId;
            if (!storesMap.count(sId)) {
                cout << "Error: Store ID not found.\n";
                continue;
            }
            cout << "Categories:\n1. Dairy\n2. Produce\n3. Bakery\n4. Meat\nSelect Category (1-4): ";
            int catChoice; cin >> catChoice;
            string catName = "Dairy";
            if (catChoice == 2) catName = "Produce";
            else if (catChoice == 3) catName = "Bakery";
            else if (catChoice == 4) catName = "Meat";
            
            cout << "Enter Quantity Sold: ";
            int qty; cin >> qty;
            
            storesMap[sId]->recordSale(catName, qty, currentDate);
            cout << "Sale processed.\n";
        }
        else if (choice == 6) {
            cout << "Enter Store ID: ";
            int sId; cin >> sId;
            if (!storesMap.count(sId)) {
                cout << "Error: Store ID not found.\n";
                continue;
            }
            dc.queryStoreInventory(sId, currentDate);
        }
        else if (choice == 7) {
            dc.processWriteOffs(currentDate);
            cout << "Write-offs processed for date " << currentDate.toString() << ".\n";
        }
        else if (choice == 8) {
            cout << "Exiting Interactive Mode.\n";
            break;
        }
        else {
            cout << "Invalid choice. Try again.\n";
        }
    }
}

int main() {
    cout << "========================================================\n";
    cout << "Welcome to FreshChain Retail Allocation & Markdown Engine!\n";
    cout << "========================================================\n";
    
    while (true) {
        cout << "\nMain Menu:\n";
        cout << "1. Run Automated Demo (Descriptive walkthrough with 4 Categories)\n";
        cout << "2. Enter Interactive Mode (Input your own data and test the engine)\n";
        cout << "3. Exit\n";
        cout << "Choice: ";
        
        int choice;
        if (!(cin >> choice)) {
            clearInput();
            cout << "Invalid input.\n\n";
            continue;
        }
        
        if (choice == 1) {
            runDemo();
        } else if (choice == 2) {
            runInteractiveMode();
        } else if (choice == 3) {
            cout << "Goodbye!\n";
            break;
        } else {
            cout << "Invalid choice.\n\n";
        }
    }
    return 0;
}
