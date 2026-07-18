all:
	$(MAKE) -C common

	$(MAKE) -C apps/BankingApp

clean:
	$(MAKE) -C common clean

	$(MAKE) -C apps/BankingApp clean
