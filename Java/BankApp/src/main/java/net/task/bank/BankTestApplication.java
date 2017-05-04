package net.task.bank;

import org.springframework.boot.SpringApplication;
import org.springframework.boot.autoconfigure.SpringBootApplication;


@SpringBootApplication
public class BankTestApplication {
	public static void main(String[] args) {
		//new AnnotationConfigApplicationContext(DBConfig.class);
		SpringApplication.run(BankTestApplication.class, args);
	}
}
