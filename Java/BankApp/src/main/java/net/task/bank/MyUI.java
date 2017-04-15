package net.task.bank;

import javax.servlet.annotation.WebServlet;

import com.vaadin.annotations.Theme;
import com.vaadin.annotations.VaadinServletConfiguration;
import com.vaadin.server.VaadinRequest;
import com.vaadin.server.VaadinServlet;
import com.vaadin.spring.annotation.SpringUI;
import com.vaadin.ui.*;

import java.io.File;
import java.io.FileOutputStream;
import java.time.ZoneId;
import java.util.Date;
import java.util.List;
import java.util.Optional;

/**
 * This UI is the application entry point. A UI may either represent a browser window
 * (or tab) or some part of a html page where a Vaadin application is embedded.
 * <p>
 * The UI is initialized using {@link #init(VaadinRequest)}. This method is intended to be
 * overridden to add component to the user interface and initialize non-component functionality.
 */
@SpringUI
@Theme("mytheme")
public class MyUI extends UI {

    @Override
    protected void init(VaadinRequest vaadinRequest) {
        File fileOutClients = new File("client_out.txt");
        File fileOutCredits = new File("credit_out.txt");
        File fileOut = new File("DataBase.xls");

        try (FileOutputStream outClients = new FileOutputStream(fileOutClients);
             FileOutputStream outCredits = new FileOutputStream(fileOutCredits);
             FileOutputStream outFile = new FileOutputStream(fileOut)
        ) {

            DataReader.readClients();
            DataReader.readCredits();

            DataStore.mergeDuplicate();
            DataStore.changeClientInfo();
            DataStore.outDataToTxt(outClients, outCredits);
        } catch (Exception ex) {
            ex.printStackTrace();
        }

        final VerticalLayout layout = new VerticalLayout();
        final HorizontalLayout horizontal = new HorizontalLayout();
        final HorizontalLayout horizontalCredit = new HorizontalLayout();

        final TextField name = new TextField("Name:");
        final TextField lastName = new TextField("Last name:");
        final TextField middleName = new TextField("Middle name:");
        final TextField phone = new TextField("Phone:");
        final TextField passport = new TextField("Passport:");
        final TextField oldPassport = new TextField("Old passport:");
        final DateField birthDate = new DateField("Birth Date:");

        final TextField amount = new TextField("Amount:");
        final TextField percent = new TextField("Percent:");
        final TextField paidSum = new TextField("Paid Sum:");
        final TextField needPaid = new TextField("Need to Pay:");
        final DateField closingDate = new DateField("Closing Date:");

        final TextField tempId = new TextField("-1");

        final Grid<Client> outTableClients = new Grid<>();
        final Grid<Credit> outTableCredit = new Grid<>();
        final List<Client> outClients = DataStore.clients;


        outTableCredit.setWidth("1800px");
        outTableCredit.setHeight("300px");
        outTableCredit.addColumn(Credit::getId).setCaption("ID").setWidth(100);
        outTableCredit.addColumn(Credit::getAmount).setCaption("Amount");
        outTableCredit.addColumn(Credit::getPercent).setCaption("Percent");
        outTableCredit.addColumn(Credit::getPaidSum).setCaption("Paid Sum");
        outTableCredit.addColumn(Credit::getNeedPaid).setCaption("Need Paid");
        outTableCredit.addColumn(Credit::getClosingDate).setCaption("Closing Date");
        outTableCredit.getEditor().setEnabled(true);
        outTableCredit.setStyleGenerator(credit -> {
            if (credit.isOverdue())
                return "overdue";
            else if (credit.isMayBeOverdue())
                return "mayBeOverdue";
            else
                return null;
        });

        outTableClients.setWidth("1800px");
        outTableClients.setHeight("400px");
        outTableClients.addColumn(Client::getId).setCaption("ID").setWidth(100);
        outTableClients.addColumn(Client::getName).setCaption("Name");
        outTableClients.addColumn(Client::getMiddleName).setCaption("Middle Name");
        outTableClients.addColumn(Client::getLastName).setCaption("Last Name");
        outTableClients.addColumn(Client::getPhone).setCaption("Phone");
        outTableClients.addColumn(Client::getPassport).setCaption("Passport");
        outTableClients.addColumn(Client::getOldPassport).setCaption("Old Passport");
        outTableClients.addColumn(Client::getBirthDay).setCaption("Birth Date");
        outTableClients.getEditor().setEnabled(true);
        outTableClients.setStyleGenerator(client -> {
            if (client.isDebtor())
                return "overdue";
            else if (client.isMayBeDebtor())
                return "mayBeOverdue";
            else
                return null;
        });

        outTableClients.addSelectionListener(event -> {
            Optional<Client> client = event.getFirstSelectedItem();
            client.ifPresent(client1 -> {
                outTableCredit.setItems(client1.getCredits());
                tempId.setValue(Integer.toString(client1.getId()));
            });
        });

        outTableClients.setItems(outClients);

        Button button = new Button("Add client");
        Button buttonCredit = new Button("Add credit");
        button.addClickListener(e -> {
            try {
                if (lastName.getValue().isEmpty() ||
                        middleName.getValue().isEmpty() || phone.getValue().isEmpty() ||
                        passport.getValue().isEmpty() || oldPassport.getValue().isEmpty())
                    throw new Exception();
                else {
                    Client client = new Client();
                    Integer maxId = -1;
                    for (Client clientFor : DataStore.clients)
                        if (maxId < clientFor.getId() || (maxId == -1)) {
                            maxId = clientFor.getId();
                        }
                    maxId = (maxId != -1? ++maxId : 0);
                    client.setId(maxId);
                    client.setName(name.getValue());
                    client.setLastName(lastName.getValue());
                    client.setMiddleName(middleName.getValue());
                    client.setPhone(phone.getValue());
                    client.setPassport(Integer.parseInt(passport.getValue()));
                    client.setBirthday(Date.from(birthDate.getValue().atStartOfDay(ZoneId.systemDefault()).toInstant()));
                    client.setOldPassport(Integer.parseInt(oldPassport.getValue()));

                    DataStore.clients.add(client);
                    outTableClients.setItems(DataStore.clients);
                    Notification.show("Client " + name.getValue() + " was added!");
                }
            } catch (Exception ex) {
                ex.printStackTrace();
                Notification.show("Incorrect data for new client!");
            }
        });

        buttonCredit.addClickListener(e -> {
            try {
                if (tempId.getValue().isEmpty() || amount.getValue().isEmpty() ||
                        percent.getValue().isEmpty() || paidSum.getValue().isEmpty() ||
                        needPaid.getValue().isEmpty())
                    throw new Exception();
                else {
                    Credit credit = new Credit();

                    credit.setId(Integer.parseInt(tempId.getValue()));
                    credit.setAmount(Integer.parseInt(amount.getValue()));
                    credit.setPercent(percent.getValue());
                    credit.setPaidSum(Integer.parseInt(paidSum.getValue()));
                    credit.setNeedPaid(Integer.parseInt(needPaid.getValue()));
                    credit.setClosingDate(Date.from(closingDate.getValue().atStartOfDay(ZoneId.systemDefault()).toInstant()));

                    DataStore.credits.add(credit);
                    Client client = DataStore.getClient(Integer.parseInt(tempId.getValue()));
                    client.addCredit(credit);
                    Optional<Client> clientOptional = Optional.of(client);
                    clientOptional.ifPresent(client1 -> outTableCredit.setItems(client1.getCredits()));
                    Notification.show("Credit for client with id " + tempId.getValue() + " was added!");
                }
            } catch (Exception ex) {
                ex.printStackTrace();
                Notification.show("Incorrect data for new client's credit!");
            }
        });

        horizontal.addComponents(name, lastName, middleName, phone, passport, oldPassport, birthDate);
        horizontalCredit.addComponents(amount, percent, paidSum, needPaid, closingDate);
        layout.addComponents(horizontal, button, outTableClients, outTableCredit, horizontalCredit, buttonCredit);

        setContent(layout);
    }


    @WebServlet(urlPatterns = "/*", name = "MyUIServlet", asyncSupported = true)
    @VaadinServletConfiguration(ui = MyUI.class, productionMode = false)
    public static class MyUIServlet extends VaadinServlet {
    }
}
