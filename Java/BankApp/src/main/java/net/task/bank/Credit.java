package net.task.bank;

import java.time.Instant;
import java.time.LocalDate;
import java.time.ZoneOffset;
import java.util.Date;


public class Credit {
    //ID клиента, сумма, процент, уже выплаченная сумма, сумма к выплате, дата закрытия.
    private int clientID;
    private double amount, paidSum, needPaid, percent;
    private Date closingDate;

    Credit() {
    }

    void setClientId(int clientID) {
        this.clientID = clientID;
    }

    void setAmount(double amount) {
        this.amount = amount;
    }

    void setPercent(double percent) {
        this.percent = percent;
    }

    void setPaidSum(double paidSum) {
        this.paidSum = paidSum;
    }

    void setNeedPaid(double needPaid) {
        this.needPaid = needPaid;
    }

    void setClosingDate(Date closingDate) {
        this.closingDate = closingDate;
    }


    public int getClientID() {
        return this.clientID;
    }

    public double getAmount() {
        return this.amount;
    }

    public double getPercent() {
        return this.percent;
    }

    public double getPaidSum() {
        return this.paidSum;
    }

    public double getNeedPaid() {
        return this.needPaid;
    }

    public Date getClosingDate() {
        return this.closingDate;
    }

    public boolean isOverdue() {
        Instant now = Instant.now();
        return now.isAfter(this.getClosingDate().toInstant()) &&
                (this.getPaidSum() < this.getNeedPaid());
    }

    public boolean isMayBeOverdue() {
        Instant now = Instant.now();
        return now.isBefore(this.getClosingDate().toInstant()) &&
                (this.getPaidSum() < this.getNeedPaid());
    }

    @Override
    public String toString() {
        return this.getClientID() + " " + this.getAmount() + " " + this.getPercent() +
                " " + this.getPaidSum() + " " + this.getNeedPaid() + " " + this.getClosingDate();
    }
}
