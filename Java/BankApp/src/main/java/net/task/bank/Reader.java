package net.task.bank;

import java.io.File;
import java.util.List;


public interface Reader {
    List<Client> readClients(File fileClients);
    List<Credit> readCredits(File fileCredits);
}
