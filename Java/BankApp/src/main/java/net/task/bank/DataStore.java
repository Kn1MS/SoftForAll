package net.task.bank;

import java.io.FileOutputStream;
import java.io.IOException;
import java.time.Instant;
import java.util.ArrayList;
import java.util.List;


public final class DataStore {
    //Создать класс, который умеет собирать данные из файлов в общую базу данных,
    //притом дублирующиеся пользователи должны быть объединены, а буква ё должна быть заменена на е. (!)
    //Не потеряйте кредиты из аккаунтов дублей. Merge клиентов лучше оформить отдельным методом.
    public static List<Client> clients = new ArrayList<>();
    public static List<Credit> credits = new ArrayList<>();
    public static int countNull = 0;

    DataStore() {
    }

    //Пользователи содержат битые записи. Может быть несколько записей с одинаковым номером паспорта и именем фамилией.
    //Результат повторного заведения одних и тех же пользователей.
    //Может быть несколько пользователей у которых новый паспорт и старый паспорт совпадает
    //(пользователь сменил паспорт, завели новый аккаунт, притом может даже поменяться фамилия).
    public static void mergeDuplicate() {
        int countDup = 0, countError = 0;

        for (Client client1 : clients) {
            for (Client client2 : clients) {
                if (!client1.equals(client2) && !client1.isDeleted && !client2.isDeleted) {
                    if (client1.getPassport() == client2.getPassport()) {
                        for (Credit credit2 : credits) {
                            if (credit2.getId() == client2.getId()) {
                                credit2.setId(client1.getId());
                            }
                        }
                        countDup++;
                        client2.isDeleted = true;
                        break;
                    } else if (client1.getOldPassport() == client2.getPassport()) {
                        for (Credit credit2 : credits) {
                            if (credit2.getId() == client2.getId()) {
                                credit2.setId(client1.getId());
                            }
                        }
                        countError++;
                        client2.isDeleted = true;
                        break;
                    }
                }
            }
        }
        System.out.println("Number of duplicates: " + countDup + " real duplicates, " +
                countError + " confusion with passports.");
        List<Client> newClients = new ArrayList<>();
        for (Client client : clients)
            if (!client.isDeleted)
                newClients.add(client);
        clients = newClients;

        Instant now = Instant.now();
        int count = 0;

        List<Credit> newCredits = new ArrayList<>();
        for (Credit credit : credits) {
            if (DataStore.getNameFromId(credit.getId()) != null)
                newCredits.add(credit);
            if (now.isAfter(credit.getClosingDate().toInstant()) &&
                    (credit.getPaidSum() < credit.getNeedPaid())) {
                if (DataStore.getNameFromId(credit.getId()) == null) {
                    BankHelper.nullPointerCredits.add(credit);
                    count++;
                    System.out.println(count + ") Found new unpaid credit with null id!");
                }
            }
        }
        credits = newCredits;

        for (Client client : clients)
            for (Credit credit : credits)
                if (credit.getId() == client.getId())
                    client.addCredit(credit);
    }

    public static void outDataToTxt(FileOutputStream outClients, FileOutputStream outCredits) throws IOException {
        for (Client client : clients)
            outClients.write((client.toString() + "\n").getBytes());

        for (Credit credit : credits)
            outCredits.write((credit.toString() + "\n").getBytes());
    }

    public static void changeClientInfo() {
        for (Client client : clients) {
            if (client.getName().contains("ё"))
                client.setName(client.getName().replace('ё', 'е'));
            if (client.getMiddleName().contains("ё"))
                client.setMiddleName(client.getMiddleName().replace('ё', 'е'));
            if (client.getLastName().contains("ё"))
                client.setLastName(client.getLastName().replace('ё', 'е'));
        }
    }

    public static String getNameFromId(int id) {
        String result = "";
        boolean isFound = false;
        for (Client client : clients)
            if (id == client.getId()) {
                result = client.getName() + " " + client.getMiddleName() + " " + client.getLastName();
                isFound = true;
            }
        if (isFound)
            countNull++; // Счётчик кредитов, у которых нет id среди клиентов.
        return result;
    }

    public static Client getClient(int id) {
        Client clientResult = new Client();
        for (Client client : clients)
            if (id == client.getId())
                clientResult = client;
        return clientResult;
    }
}
