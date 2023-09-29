
package tstjfx;

import javafx.application.Application;
import javafx.scene.Scene;
import javafx.scene.layout.HBox;
import javafx.scene.paint.Color;
import javafx.scene.shape.Line;
import javafx.stage.Stage;

public class Fx2DShape extends Application
{
  public static void main(String[] args)
  {
    Application.launch(args);
  }
  @Override
  public void start(Stage stage)
  {
    // Create the Lines
    Line line1 = new Line();
    Line line2 = new Line(0, 0, 150, 0);
    line2.setStrokeWidth(1.0);
    Line line3 = new Line(0, 150, 150, 0);
    line3.setStrokeWidth(2.0);
    line3.setStroke(Color.RED);
    Line line4 = new Line(0, 0, 150, 150);
    line4.setStrokeWidth(5.0);
    line4.setStroke(Color.BLUE);
    // Create the HBox
    HBox root = new HBox();
    // Add the Children to the HBox
    root.getChildren().addAll(line1, line2, line3, line4);
    // Set Spacing of the HBox
    root.setSpacing(10);
    // Set the Style of the HBox
    root.setStyle
    (
      "-fx-padding: 10;" +
      "-fx-border-style: solid inside;" +
      "-fx-border-width: 1;" +
      "-fx-border-insets: 5;" +
      "-fx-border-radius: 5;" +
      "-fx-border-color: blue;"
    );
    // Create the Scene
    Scene scene = new Scene(root);
    // Add the Scene to the Stage
    stage.setScene(scene);
    // Set the Title of the Stage
    stage.setTitle("A JavaFX Line Example");
    // Display the Stage
    stage.show();
  }
}
