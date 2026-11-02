import java.util.*;

// 1. SENSOR
interface Sensor {
    String type();
    double read();
}

class SoilMoistureSensor implements Sensor {
    Scanner sc;

    SoilMoistureSensor(Scanner sc) {
        this.sc = sc;
    }

    public String type() {
        return "Soil Moisture Sensor";
    }

    public double read() {
        System.out.print("Enter soil moisture (%): ");
        return sc.nextDouble();
    }
}

class TemperatureSensor implements Sensor {
    Scanner sc;

    TemperatureSensor(Scanner sc) {
        this.sc = sc;
    }

    public String type() {
        return "Temperature Sensor";
    }

    public double read() {
        System.out.print("Enter temperature: ");
        return sc.nextDouble();
    }
}

// 2. FACTORY PATTERN
class SensorFactory {
    Scanner sc;

    SensorFactory(Scanner sc) {
        this.sc = sc;
    }

    Sensor createSensor(int choice) {
        if (choice == 1)
            return new SoilMoistureSensor(sc);
        else if (choice == 2)
            return new TemperatureSensor(sc);
        else
            return null;
    }
}

// 3. WEATHER ADAPTER PATTERN
class WeatherData {
    boolean rainExpected;

    WeatherData(boolean rain) {
        rainExpected = rain;
    }
}

interface WeatherService {
    WeatherData current();
}

class ExternalWeatherAPI {
    boolean getRainForecast(boolean rain) {
        return rain;
    }
}

class WeatherAPIAdapter implements WeatherService {
    ExternalWeatherAPI api;
    boolean rain;

    WeatherAPIAdapter(ExternalWeatherAPI api, boolean rain) {
        this.api = api;
        this.rain = rain;
    }

    public WeatherData current() {
        return new WeatherData(api.getRainForecast(rain));
    }
}

// 4. STRATEGY PATTERN
interface IrrigationStrategy {
    boolean shouldIrrigate(double moisture, WeatherData weather);
}

class NormalIrrigationStrategy implements IrrigationStrategy {
    public boolean shouldIrrigate(double moisture, WeatherData weather) {
        return moisture < 40 && !weather.rainExpected;
    }
}

class WaterSavingStrategy implements IrrigationStrategy {
    public boolean shouldIrrigate(double moisture, WeatherData weather) {
        return moisture < 25 && !weather.rainExpected;
    }
}

// 5. IRRIGATION PUMP
class IrrigationPump {
    void setOn(boolean on) {
        System.out.println(on ? "Pump is ON" : "Pump is OFF");
    }
}

// 6. OBSERVER PATTERN
interface AlertObserver {
    void update(String message);
}

class FarmerNotifier implements AlertObserver {
    public void update(String message) {
        System.out.println("Farmer: " + message);
    }
}

class FarmDashboard implements AlertObserver {
    public void update(String message) {
        System.out.println("Dashboard: " + message);
    }
}

class AlertPublisher {
    List<AlertObserver> observers = new ArrayList<>();

    void subscribe(AlertObserver o) {
        observers.add(o);
    }

    void publish(String message) {
        for (AlertObserver o : observers)
            o.update(message);
    }
}

// 7. FIELD AND FARM DATA MANAGER
class Field {
    String name;

    Field(String name) {
        this.name = name;
    }
}

class FarmDataManager {
    List<Field> fields = new ArrayList<>();

    void addField(Field f) {
        fields.add(f);
        System.out.println("Field added successfully!");
    }

    void showFields() {
        if (fields.isEmpty()) {
            System.out.println("No fields available.");
            return;
        }

        for (Field f : fields)
            System.out.println("Field: " + f.name);
    }
}

// 8. IRRIGATION CONTROLLER
class IrrigationController {
    IrrigationStrategy strategy;
    WeatherService weather;
    IrrigationPump pump;
    AlertPublisher alerts;

    IrrigationController(IrrigationStrategy s, WeatherService w,
                         IrrigationPump p, AlertPublisher a) {
        strategy = s;
        weather = w;
        pump = p;
        alerts = a;
    }

    void check(Sensor sensor) {
        double moisture = sensor.read();
        WeatherData data = weather.current();

        System.out.println("\nSensor: " + sensor.type());
        System.out.println("Reading: " + moisture);
        System.out.println("Rain Expected: " + data.rainExpected);

        boolean result = strategy.shouldIrrigate(moisture, data);

        pump.setOn(result);

        if (result)
            alerts.publish("Irrigation Started!");
        else
            alerts.publish("Irrigation Not Required!");
    }
}

// 9. MAIN CLASS
public class AASExam {
    public static void main(String[] args) {

        Scanner sc = new Scanner(System.in);

        SensorFactory factory = new SensorFactory(sc);
        ExternalWeatherAPI api = new ExternalWeatherAPI();
        IrrigationPump pump = new IrrigationPump();
        AlertPublisher publisher = new AlertPublisher();
        FarmDataManager manager = new FarmDataManager();

        // Automatically register observers
        publisher.subscribe(new FarmerNotifier());
        publisher.subscribe(new FarmDashboard());

        IrrigationStrategy strategy = new NormalIrrigationStrategy();

        while (true) {
            System.out.println("\n===== SMART IRRIGATION SYSTEM =====");
            System.out.println("1. Add Farm Field");
            System.out.println("2. View Farm Fields");
            System.out.println("3. Select Irrigation Strategy");
            System.out.println("4. Check Irrigation");
            System.out.println("5. Exit");

            System.out.print("Enter choice: ");
            int choice = sc.nextInt();
            sc.nextLine();

            switch (choice) {

                case 1:
                    System.out.print("Enter field name: ");
                    String name = sc.nextLine();
                    manager.addField(new Field(name));
                    break;

                case 2:
                    manager.showFields();
                    break;

                case 3:
                    System.out.println("1. Normal Strategy");
                    System.out.println("2. Water Saving Strategy");
                    System.out.print("Choose strategy: ");

                    int s = sc.nextInt();

                    if (s == 1) {
                        strategy = new NormalIrrigationStrategy();
                        System.out.println("Normal Strategy Selected!");
                    }
                    else if (s == 2) {
                        strategy = new WaterSavingStrategy();
                        System.out.println("Water Saving Strategy Selected!");
                    }
                    else {
                        System.out.println("Invalid choice!");
                    }
                    break;

                case 4:
                    System.out.println("1. Soil Moisture Sensor");
                    System.out.println("2. Temperature Sensor");
                    System.out.print("Choose sensor: ");

                    Sensor sensor = factory.createSensor(sc.nextInt());

                    if (sensor == null) {
                        System.out.println("Invalid sensor choice!");
                        break;
                    }

                    System.out.print("Rain expected? (1=Yes, 0=No): ");
                    int rain = sc.nextInt();

                    if (rain != 0 && rain != 1) {
                        System.out.println("Invalid weather choice!");
                        break;
                    }

                    WeatherService weather =
                        new WeatherAPIAdapter(api, rain == 1);

                    IrrigationController controller =
                        new IrrigationController(strategy, weather, pump, publisher);

                    controller.check(sensor);
                    break;

                case 5:
                    System.out.println("Thank you for using Smart Irrigation System!");
                    sc.close();
                    return;

                default:
                    System.out.println("Invalid choice!");
            }
        }
    }
}