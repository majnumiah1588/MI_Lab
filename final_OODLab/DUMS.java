import java.util.*;

// Complaint Class
class Complaint {
    int id;
    String citizen, type, location, status;

    Complaint(int id, String citizen, String type, String location) {
        this.id = id;
        this.citizen = citizen;
        this.type = type;
        this.location = location;
        status = "Pending";
    }

    void display() {
        System.out.println("ID: " + id + " | Citizen: " + citizen
                + " | Type: " + type + " | Location: " + location
                + " | Status: " + status);
    }
}

// 1. Repository Pattern
class UrbanDatabase {
    ArrayList<Complaint> complaints = new ArrayList<>();

    void addComplaint(Complaint c) {
        complaints.add(c);
    }

    Complaint findComplaint(int id) {
        for (Complaint c : complaints) {
            if (c.id == id)
                return c;
        }
        return null;
    }

    void showAll() {
        for (Complaint c : complaints)
            c.display();
    }

    void showCitizen(String name) {
        for (Complaint c : complaints) {
            if (c.citizen.equalsIgnoreCase(name))
                c.display();
        }
    }
}

// 2. Factory Pattern
interface UrbanService {
    void provideService();
}

class WasteService implements UrbanService {
    public void provideService() {
        System.out.println("Waste service selected.");
    }
}

class TrafficService implements UrbanService {
    public void provideService() {
        System.out.println("Traffic service selected.");
    }
}

class EmergencyService implements UrbanService {
    public void provideService() {
        System.out.println("Emergency service selected.");
    }
}

class UrbanServiceFactory {
    static UrbanService create(String type) {
        if (type.equalsIgnoreCase("waste"))
            return new WasteService();
        if (type.equalsIgnoreCase("traffic"))
            return new TrafficService();
        if (type.equalsIgnoreCase("emergency"))
            return new EmergencyService();

        return null;
    }
}

// 3. Observer Pattern
interface ComplaintObserver {
    void notifyUser(String message);
}

class CitizenNotifier implements ComplaintObserver {
    public void notifyUser(String message) {
        System.out.println("Citizen Notification: " + message);
    }
}

class GovernmentDashboard implements ComplaintObserver {
    public void notifyUser(String message) {
        System.out.println("Government Dashboard: " + message);
    }
}

class StatusPublisher {
    ArrayList<ComplaintObserver> observers = new ArrayList<>();

    void addObserver(ComplaintObserver o) {
        observers.add(o);
    }

    void notifyAllUsers(String message) {
        for (ComplaintObserver o : observers)
            o.notifyUser(message);
    }
}

// 4. Strategy Pattern
interface RouteStrategy {
    void findRoute();
}

class ShortestRoute implements RouteStrategy {
    public void findRoute() {
        System.out.println("Shortest Route: Road A");
        System.out.println("Distance: 5 km | Estimated Time: 20 minutes");
    }
}

class FastestRoute implements RouteStrategy {
    public void findRoute() {
        System.out.println("Fastest Route: Road B");
        System.out.println("Distance: 8 km | Estimated Time: 12 minutes");
    }
}

class RoutePlanner {
    RouteStrategy strategy;

    void setStrategy(RouteStrategy s) {
        strategy = s;
    }

    void planRoute() {
        strategy.findRoute();
    }
}

// 5. Adapter Pattern
class ExternalTrafficAPI {
    String getTrafficData() {
        return "Road A: Low Traffic, Road B: High Traffic, Road C: Medium Traffic";
    }
}

interface TrafficServiceAPI {
    void showTrafficData();
}

class TrafficAPIAdapter implements TrafficServiceAPI {
    ExternalTrafficAPI api = new ExternalTrafficAPI();

    public void showTrafficData() {
        System.out.println("\nTraffic Information:");
        System.out.println(api.getTrafficData());
    }
}

// Citizen Role
class Citizen {
    void menu(UrbanDatabase db, StatusPublisher publisher) {
        Scanner sc = new Scanner(System.in);

        System.out.print("Enter your name: ");
        String name = sc.nextLine();

        int choice;

        do {
            System.out.println("\n--- Citizen Menu ---");
            System.out.println("1. Submit Complaint");
            System.out.println("2. View My Complaints");
            System.out.println("3. Exit");
            System.out.print("Choice: ");
            choice = sc.nextInt();
            sc.nextLine();

            if (choice == 1) {
                System.out.print("Complaint ID: ");
                int id = sc.nextInt();
                sc.nextLine();

                if (db.findComplaint(id) != null) {
                    System.out.println("Complaint ID already exists.");
                    continue;
                }

                System.out.print("Service (waste/traffic/emergency): ");
                String type = sc.nextLine();

                UrbanService service = UrbanServiceFactory.create(type);

                if (service == null) {
                    System.out.println("Invalid service.");
                    continue;
                }

                service.provideService();

                System.out.print("Location: ");
                String location = sc.nextLine();

                db.addComplaint(new Complaint(id, name, type, location));
                publisher.notifyAllUsers("New complaint submitted. ID: " + id);
            }

            else if (choice == 2) {
                db.showCitizen(name);
            }

        } while (choice != 3);
    }
}

// Administrator Role
class Administrator {
    void menu(UrbanDatabase db, StatusPublisher publisher) {
        Scanner sc = new Scanner(System.in);
        int choice;

        do {
            System.out.println("\n--- Administrator Menu ---");
            System.out.println("1. View All Complaints");
            System.out.println("2. Update Complaint Status");
            System.out.println("3. Plan Route");
            System.out.println("4. View Traffic Data");
            System.out.println("5. Exit");
            System.out.print("Choice: ");
            choice = sc.nextInt();

            if (choice == 1) {
                db.showAll();
            }

            else if (choice == 2) {
                System.out.print("Enter Complaint ID: ");
                int id = sc.nextInt();

                Complaint c = db.findComplaint(id);

                if (c != null) {
                    c.status = "Resolved";
                    System.out.println("Complaint status updated to Resolved.");
                    publisher.notifyAllUsers("Complaint " + id + " is now Resolved.");
                } else {
                    System.out.println("Complaint not found.");
                }
            }

            else if (choice == 3) {
                System.out.println("1. Shortest Route");
                System.out.println("2. Fastest Route");
                System.out.print("Choose route: ");
                int option = sc.nextInt();

                RoutePlanner planner = new RoutePlanner();

                if (option == 1)
                    planner.setStrategy(new ShortestRoute());
                else if (option == 2)
                    planner.setStrategy(new FastestRoute());
                else {
                    System.out.println("Invalid choice.");
                    continue;
                }

                planner.planRoute();
            }

            else if (choice == 4) {
                TrafficServiceAPI traffic = new TrafficAPIAdapter();
                traffic.showTrafficData();
            }

        } while (choice != 5);
    }
}

// Main Class
public class DUMS {
    public static void main(String[] args) {
        Scanner sc = new Scanner(System.in);

        UrbanDatabase db = new UrbanDatabase();

        StatusPublisher publisher = new StatusPublisher();
        publisher.addObserver(new CitizenNotifier());
        publisher.addObserver(new GovernmentDashboard());

        while (true) {
            System.out.println("\n===== DUMS =====");
            System.out.println("1. Citizen");
            System.out.println("2. Administrator");
            System.out.println("3. Exit");
            System.out.print("Select Role: ");

            int choice = sc.nextInt();
            sc.nextLine();

            if (choice == 1)
                new Citizen().menu(db, publisher);

            else if (choice == 2)
                new Administrator().menu(db, publisher);

            else if (choice == 3) {
                System.out.println("Program ended.");
                break;
            }

            else
                System.out.println("Invalid choice.");
        }
    }
}