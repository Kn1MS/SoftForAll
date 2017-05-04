package net.task.bank;


import org.springframework.context.annotation.Bean;
import org.springframework.context.annotation.Configuration;

@Configuration
public class BankTestAppConfig {
    @Bean
    public Reader getReader() {
        return new DataReader();
    }
}
