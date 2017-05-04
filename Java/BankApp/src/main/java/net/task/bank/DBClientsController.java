package net.task.bank;

import java.util.List;

public interface DBClientsController {
    void saveNewClient(Client toCreate);
    List<Client> getAllClients();
    void deleteClient(int clientID);
    Client getByID(int clientID);
    void updateClient(Client toUpdate);
}