package net.task.bank;

import org.springframework.jdbc.core.RowMapper;

import java.sql.ResultSet;
import java.sql.SQLException;


public class ClientRowMapper implements RowMapper<Client> {
    @Override
    public Client mapRow(ResultSet resultSet, int i) throws SQLException {
        Client client = new Client();
        client.setID(resultSet.getInt("ID"));
        client.setFirstName(resultSet.getString("firstName"));
        client.setMiddleName(resultSet.getString("middleName"));
        client.setLastName(resultSet.getString("lastName"));
        client.setPhone(resultSet.getString("phone"));
        client.setPassport(resultSet.getInt("passport"));
        client.setOldPassport(resultSet.getInt("oldPassport"));
        client.setBirthday(resultSet.getTimestamp("birthDay"));

        return client;
    }
}
