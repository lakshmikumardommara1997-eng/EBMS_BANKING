all:
	$(MAKE) -C common
	$(MAKE) -C customer
	$(MAKE) -C apps/BankingApp

clean:
	$(MAKE) -C common clean
	$(MAKE) -C customer clean
	$(MAKE) -C apps/BankingApp clean

	