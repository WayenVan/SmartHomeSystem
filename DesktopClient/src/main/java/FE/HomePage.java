package FE;

import javax.swing.*;
import java.awt.*;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.awt.image.BufferedImage;
import java.io.IOException;
import java.io.OutputStream;
import java.net.Socket;
import java.nio.ByteBuffer;

/**
 * @author by Liangyue
 * @date 15/04/2021.
 */

public class HomePage extends JFrame implements ActionListener {

    private JFrame jFrame = new JFrame();
    private JButton unlock = new JButton("Unlock");
    private JLabel label = new JLabel();
    private JLabel labelTemp = new JLabel();
    private JLabel labelHumi = new JLabel();
    private JLabel labelPres = new JLabel();
    private JPanel panel = new JPanel();
    private String host = JOptionPane.showInputDialog("Please input the host");
    private String port = JOptionPane.showInputDialog("Please input the port");
    private static int lastStatus = 0;

    public HomePage() throws HeadlessException {
        super("Homepage");
        super.setTitle("Viewer Homepage");
        super.setLocationRelativeTo(null);
        super.setSize(800, 600);
        super.setPreferredSize(new Dimension(150, 25));
        super.add(panel);
        placeComponents(panel);
        super.setVisible(true);
    }

    private void placeComponents(JPanel panel) {
        GridBagLayout layout = new GridBagLayout();
        GridBagConstraints c = new GridBagConstraints();
        panel.setLayout(layout);

//        // Image
//        c.fill = GridBagConstraints.HORIZONTAL;
//        c.gridy = 0;
//        c.gridx = 0;
//        c.gridwidth = 3;
//        c.gridheight = 3;
//        c.ipady = 600;
//        c.ipadx = 800;
//        labelPres.setHorizontalAlignment(SwingConstants.CENTER);
//        panel.add(label, c);

        // Temperature
        c = new GridBagConstraints();
        c.fill = GridBagConstraints.HORIZONTAL;
        c.weightx = 0.5;
        c.gridy = 1;
        c.gridx = 0;
        labelTemp.setHorizontalAlignment(SwingConstants.CENTER);
        panel.add(labelTemp, c);

        // Humidity
        c = new GridBagConstraints();
        c.fill = GridBagConstraints.HORIZONTAL;
        c.weightx = 0.5;
        c.gridy = 1;
        c.gridx = 1;
        labelHumi.setHorizontalAlignment(SwingConstants.CENTER);
        panel.add(labelHumi, c);

        // Pressure
        c = new GridBagConstraints();
        c.fill = GridBagConstraints.HORIZONTAL;
        c.weightx = 0.5;
        c.gridy = 1;
        c.gridx = 2;
        labelPres.setHorizontalAlignment(SwingConstants.CENTER);
        panel.add(labelPres, c);

        c = new GridBagConstraints();
        c.fill = GridBagConstraints.HORIZONTAL;
        c.gridy = 2;
        c.gridx = 1;
        c.ipady = 40;
        c.insets = new Insets(20, 0, 0, 0);
        panel.add(unlock, c);
        unlock.addActionListener(this);
    }

    public void showImage(BufferedImage bufferedImage){
        ImageIcon image = new ImageIcon(bufferedImage);
        label.setIcon(image);
        jFrameImage(label);
        label.invalidate();
        label.repaint();
    }

    private void jFrameImage(JLabel label) {
        jFrame.setTitle("Face Recognition");
        jFrame.setSize(800, 600);
        jFrame.add(label);
        jFrame.setVisible(true);
    }

    public void showTemp(double doubleTemp){
        String stringTemp = String.valueOf(doubleTemp);
        labelTemp.setText(stringTemp + " ℃");
        labelTemp.invalidate();
        labelTemp.repaint();
    }

    public void showHumi(double doubleHumi){
        String stringHumi = String.valueOf(doubleHumi);
        labelHumi.setText(stringHumi + " %rh");
        labelHumi.invalidate();
        labelHumi.repaint();
    }

    public void showPres(double doublePres){
        String stringPres = String.valueOf(doublePres);
        labelPres.setText(stringPres + " Pa");
        labelPres.invalidate();
        labelPres.repaint();
    }

    @Override
    public void actionPerformed(ActionEvent e) {
        if (e.getSource() == unlock) {
            try {
                // host: 192.168.0.100 port: 12345
                String host = getHost();
                Socket socket = new Socket(host, 1148);
                // Output stream
                OutputStream os = socket.getOutputStream();
                int mark = 0x15;
                mark = Integer.reverseBytes(mark);
                os.write(ByteBuffer.allocate(4).putInt(mark).array());

            } catch (IOException exception) {
                exception.printStackTrace();
            }
        }
    }

    public void showLocker(int intLocker) {
        if (intLocker > 0 && lastStatus == 0){
            // 1 means door opens
            JOptionPane.showMessageDialog(null,
                    "The door opens", "Information",JOptionPane.ERROR_MESSAGE);
        } else {
        }
    }

    public void getLastStatus(int status) {
        lastStatus = status;
    }

    public String getHost() {
        return host;
    }

    public void setHost(String host) {
        this.host = host;
    }

    public String getPort() {
        return port;
    }

    public void setPort(String port) {
        this.port = port;
    }
}
