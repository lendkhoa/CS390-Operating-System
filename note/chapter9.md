# Virtual memory
> April 12, 2017

![](image/virtualMem1.png)

![](image/sharedLib.png)

### swapper vs pager (demand paging)
**lazy swapper** never swaps a page into memory unless that page will be needed.
- A swapper manipulates the entire processes, whereas a **pager** is concerned with the individual pages of a process.

![](image/transferPage.png)

**page fault:** occurs when a proc generate an address that falls on a page that is not in memory

![](image/pageNotInMain.png)

**pure demand paging** never bring a page into memory until it is required

The hardware to support demand paging is the same as the hardware for paging and swapping:

