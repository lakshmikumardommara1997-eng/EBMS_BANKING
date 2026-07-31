all:
	$(MAKE) -C common
	$(MAKE) -C customer
	$(MAKE) -C utils
	$(MAKE) -C Logger
	$(MAKE) -C apps/BankingApp

clean:
	$(MAKE) -C common clean
	
	$(MAKE) -C customer clean
	$(MAKE) -C utils clean
	$(MAKE) -C Logger clean
	$(MAKE) -C apps/BankingApp clean
	

	