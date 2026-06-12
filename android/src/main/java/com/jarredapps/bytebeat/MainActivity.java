package com.jarredapps.bytebeat;

import android.Manifest;
import android.app.Activity;
import android.app.AlertDialog;
import android.app.Dialog;
import android.app.ProgressDialog;
import android.content.Context;
import android.content.DialogInterface;
import android.content.Intent;
import android.content.pm.PackageManager;
import android.database.Cursor;
import android.graphics.Color;
import android.net.Uri;
import android.os.Build;
import android.os.Bundle;
import android.os.Handler;
import android.os.Looper;
import android.os.ParcelFileDescriptor;
import android.os.VibrationEffect;
import android.os.Vibrator;
import android.provider.OpenableColumns;
import android.view.LayoutInflater;
import android.view.Menu;
import android.view.MenuItem;
import android.view.OrientationEventListener;
import android.view.View;
import android.widget.AdapterView;
import android.widget.ArrayAdapter;
import android.widget.Button;
import android.widget.EditText;
import android.widget.ImageButton;
import android.widget.ImageView;
import android.widget.LinearLayout;
import android.widget.ListView;
import android.widget.Spinner;
import android.widget.Toast;
import android.widget.Toolbar;
import java.io.BufferedReader;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.io.InputStreamReader;
import java.io.OutputStreamWriter;
import java.util.ArrayList;

public class MainActivity extends Activity {
    private Toolbar mainToolbar;

    private Intent openFile = new Intent(Intent.ACTION_OPEN_DOCUMENT);
    private Intent saveFile = new Intent(Intent.ACTION_CREATE_DOCUMENT);
    private Intent exportFile = new Intent(Intent.ACTION_CREATE_DOCUMENT);
    private final int OPENFILE_CODE = 1001;
	private final int SAVEFILE_CODE = 1002;
    private final int EXPORTFILE_CODE = 1003;

    private Button mainPlay;
    private Button mainStop;
    private EditText mainFrequencyEdit;
    private Spinner mainBytebeatType;

    private int currentRotation = 0;

    int typePos;
    int secondsInt;
    int fabClicked;
    boolean isNew;

    private String subtitle = "untitled";

    private EditText mainEditFormula;

    private BytebeatPlayer player;

    private ProgressDialog exportDialog;

    private LinearLayout mainOutputLog;
    private ListView mainListOutput;
    private ImageButton fab;

    private ArrayList<String> logList = new ArrayList<>();

    //private OrientationEventListener orientationEventListener;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.main);

        isNew = false;

        if (Build.VERSION.SDK_INT >= 23) {
            if (checkSelfPermission(Manifest.permission.READ_EXTERNAL_STORAGE) == PackageManager.PERMISSION_DENIED 
                && checkSelfPermission(Manifest.permission.WRITE_EXTERNAL_STORAGE) == PackageManager.PERMISSION_DENIED
                && checkSelfPermission(Manifest.permission.MANAGE_EXTERNAL_STORAGE) == PackageManager.PERMISSION_DENIED) {
                requestPermissions(new String[] {
                                       Manifest.permission.READ_EXTERNAL_STORAGE, 
                                       Manifest.permission.WRITE_EXTERNAL_STORAGE,
                                       Manifest.permission.MANAGE_EXTERNAL_STORAGE}, 
                                   1000);
            } else {
                initializeLogic();
                restoreBundle(savedInstanceState);
            }
        } else {
            initializeLogic();
            restoreBundle(savedInstanceState);
            
		}


    }

    

    @Override
    public void onRequestPermissionsResult(int requestCode, String[] permissions, int[] grantResults) {
        super.onRequestPermissionsResult(requestCode, permissions, grantResults);
        if (requestCode == 1000) {
            initializeLogic();
        }
	}

    private void initializeLogic() {
        openFile.addCategory(Intent.CATEGORY_OPENABLE);
        openFile.setType("text/plain");
        openFile.putExtra(Intent.EXTRA_ALLOW_MULTIPLE, false);
        String[] mimeTypes = {"text/plain"};
        openFile.putExtra(Intent.EXTRA_MIME_TYPES, mimeTypes);

        saveFile.addCategory(Intent.CATEGORY_OPENABLE);
        saveFile.setType("text/plain");
        //saveFile.putExtra(Intent.EXTRA_ALLOW_MULTIPLE, false);
        String[] mimeTypes2 = {"text/plain"};
        saveFile.putExtra(Intent.EXTRA_MIME_TYPES, mimeTypes2);

        exportFile.addCategory(Intent.CATEGORY_OPENABLE);
        exportFile.setType("audio/wav");

        mainToolbar = findViewById(R.id.mainToolbar);
        mainToolbar.setSubtitleTextColor(Color.GRAY);

        setActionBar(mainToolbar);
        getActionBar().setLogo(R.drawable.ic_launcher);
        getActionBar().setTitle(R.string.app_name);
        getActionBar().setSubtitle(subtitle);

        setTitleColor(Color.BLACK);

        mainPlay = findViewById(R.id.mainPlay);
        mainStop = findViewById(R.id.mainStop);
        mainFrequencyEdit = findViewById(R.id.mainFrequencyEdit);
        mainBytebeatType = findViewById(R.id.mainBytebeatType);

        mainEditFormula = findViewById(R.id.mainEditFormula);

        mainOutputLog = findViewById(R.id.mainOutputLog);
        mainListOutput = findViewById(R.id.mainListOutput);
        fab = findViewById(R.id.fab);

        // set defaults
        if (mainFrequencyEdit != null) {
            mainFrequencyEdit.setText("8000");
        }
        if (mainEditFormula != null) {
            mainEditFormula.setText("t*(42&t>>10)");
        }
        if (mainOutputLog != null) {
            mainOutputLog.setVisibility(View.INVISIBLE);
        }
        if (mainListOutput != null) {
            setLogDefaults();
            //refreshLog(logList);
        }

        if (mainBytebeatType != null) {
            ArrayList<String> list = new ArrayList<>();
//            if (!list.isEmpty()) {
//                list.clear();
//            }
            list.add("Bytebeat");
            list.add("Signed Bytebeat");

            final ArrayAdapter<String> listStr = new ArrayAdapter<>(
                MainActivity.this, 
                android.R.layout.simple_list_item_1,
                list);

            mainBytebeatType.setAdapter(listStr);
            runOnUiThread(new Runnable(){
                    @Override
                    public void run() {
                        listStr.notifyDataSetChanged();
                    }
                });
            mainBytebeatType.setOnItemSelectedListener(new Spinner.OnItemSelectedListener() {

                    @Override
                    public void onItemSelected(AdapterView<?> parent, View view, int position, long id) {                 
                        typePos = position;
                    }

                    @Override
                    public void onNothingSelected(AdapterView<?> parent) {
                        typePos = 0;
                    }

                });
        }

        player = new BytebeatPlayer();

        if (mainPlay != null) {
            mainPlay.setOnClickListener(new View.OnClickListener() {
                    @Override
                    public void onClick(View view) {
                        String formula =
                            mainEditFormula.getText().toString().trim();

                        String frequency =
                            mainFrequencyEdit.getText().toString().trim();

                        if (formula.isEmpty()) {
                            vibrateErrorPattern();
                            doErrorAnimation(mainEditFormula);
                            mainEditFormula.setError("Formula is Empty!");
                            logList.add("Formula is Empty!");
                            refreshLog(logList);
                            return;
                        }

                        if (frequency.isEmpty()) {
                            vibrateErrorPattern();
                            doErrorAnimation(mainFrequencyEdit);
                            mainFrequencyEdit.setError(
                                "You must enter a frequency");
                            logList.add("You must enter a frequency");
                            refreshLog(logList);
                            return;
                        }

                        try {

                            int freqInt =
                                Integer.parseInt(frequency);

                            if (typePos == 0) {
                                player.start(
                                    formula,
                                    freqInt,
                                    BytebeatType.BYTEBEAT_UNSIGNED
                                );

                                logList.add("Playing: " + formula);
                                refreshLog(logList);
                            } else if (typePos == 1) {
                                player.start(
                                    formula,
                                    freqInt,
                                    BytebeatType.BYTEBEAT_SIGNED
                                );

                                logList.add("Playing: " + formula);
                                refreshLog(logList);
                            }
                        } catch (NumberFormatException e) {
                            vibrateErrorPattern();
                            doErrorAnimation(mainFrequencyEdit);
                            mainFrequencyEdit.setError(
                                "Frequency must be numeric");
                                
                            logList.add("Frequency must be numeric");
                            refreshLog(logList);
                            return;

                        } catch (RuntimeException e) {           
                            vibrateErrorPattern();
                            e.printStackTrace();
                            doErrorAnimation(mainEditFormula);
                            mainEditFormula.setError("java.lang.RuntimeException Error: " 
                                                     + e.toString());

                            logList.add("java.lang.RuntimeException Error: " 
                                        + e.toString());
                            refreshLog(logList);
                            return;
                        }
                    }
                });
        }
        if (mainStop != null) {
            mainStop.setOnClickListener(new View.OnClickListener() {

                    @Override
                    public void onClick(View view) {
                        player.stop();           

                        logList.add("Stopped");
                        refreshLog(logList);
                    }
                });
        }

        fab.setOnClickListener(new View.OnClickListener() {
                @Override
                public void onClick(View view) {
                    fabClicked++;

                    if (fabClicked == 1) {
                        fab.setImageResource(R.drawable.ic_arrow_down);

                        mainOutputLog.setVisibility(View.VISIBLE);

                        mainOutputLog.animate()
                            .alpha(1.0f)
                            .setDuration(200)
                            .start();
                    } else if (fabClicked == 2) {
                        fab.setImageResource(R.drawable.ic_output);                     
                        fabClicked = 0;

                        new Handler(Looper.getMainLooper()).postDelayed(new Runnable(){

                                @Override
                                public void run() {
                                    mainOutputLog.animate()
                                        .alpha(0.0f)
                                        .setDuration(200)
                                        .start();
                                }
                            }, 200);

                        mainOutputLog.setVisibility(View.INVISIBLE);
                    }
                }
            });

//        orientationEventListener =
//            new OrientationEventListener(this) {
//
//            @Override
//            public void onOrientationChanged(int orientation) {
//
//                if (orientation == ORIENTATION_UNKNOWN)
//                    return;
//
//                int rotation;
//
//                // Orientation is returned in degrees (0-359)
//                // 0: Portrait (natural)
//                // 90: Landscape (left side up)
//                // 180: Upside down
//                // 270: Landscape (right side up)
//                if (orientation >= 315 || orientation < 45) {
//                    rotation = 0;
//                } else if (orientation < 135) {
//                    rotation = 90;
//                } else if (orientation < 225) {
//                    rotation = 180;
//                } else {
//                    rotation = 270;
//                }
//
//
//                if (rotation != currentRotation) {
//                    currentRotation = rotation;
//
//                    // LANDSCAPE
//                    if (currentRotation == 90) {
//
//                    }
//                    if (currentRotation == 270) {
//                    }
//
//                    // PORTRAIT
//                    if (currentRotation == 180) {
//                    }
//                    if (currentRotation == 0) {
//                    }
//                }
//            }
//        };
//
//        orientationEventListener.enable();
    }


    @Override
    protected void onSaveInstanceState(Bundle outState) {

        super.onSaveInstanceState(outState);
        
        outState.putString("filenameSubtitle",
            subtitle);

        outState.putString(
            "formula",
            mainEditFormula.getText().toString());

        outState.putString(
            "frequency",
            mainFrequencyEdit.getText().toString());

        outState.putInt(
            "bytebeatType", typePos);

        outState.putStringArrayList(
            "log", logList);

    }
    
    private void restoreBundle(Bundle savedInstanceState) {
        if (savedInstanceState != null) {
            
            getActionBar().setSubtitle(
            savedInstanceState.getString("filenameSubtitle", "untitled"));      

            mainEditFormula.setText(
                savedInstanceState.getString(
                    "formula",
                    ""));

            mainFrequencyEdit.setText(
                savedInstanceState.getString(
                    "frequency",
                    "8000"));

            mainBytebeatType.setSelection(
                savedInstanceState.getInt("bytebeatType"));

            refreshLog(
                savedInstanceState.getStringArrayList("log"));                 
        }
    }

//	void doShakeOnFormula() {
//		if (mainEditFormula != null) {
//			mainEditFormula.animate()
//			.translationX(-10f)
//			.setDuration(100)
//			.setInterpolator(new AccelerateInterpolator())
//			.start();
//			
//			mainEditFormula.animate()
//				.translationX(10f)
//				.setDuration(100)
//				.setInterpolator(new AccelerateInterpolator())
//				.start();
//				
//			mainEditFormula.animate()
//				.translationX(-10f)
//				.setDuration(100)
//				.setInterpolator(new AccelerateInterpolator())
//				.start();
//				
//			mainEditFormula.animate()
//				.translationX(10f)
//				.setDuration(100)
//				.setInterpolator(new AccelerateInterpolator())
//				.start();
//				
//			mainEditFormula.animate()
//				.translationX(0f)
//				.setDuration(230)
//				.setInterpolator(new AccelerateInterpolator())
//				.start();
//			
////			// Define start and end points using floating-point positions (e.g., 0f to 300f pixels)
////			float startX = mainEditFormula.getTranslationX();
////			float endX = 450.5f; 
////
////			// Create the animator specifically targeting "translationX"
////			ObjectAnimator animator = ObjectAnimator.ofFloat(mainEditFormula, "translationX", startX, endX);
////
////			// Set the duration in milliseconds
////			animator.setDuration(400); 
////
////			// This specific interpolator ensures soft, natural start/stop transitions with zero shake
////			animator.setInterpolator(new AccelerateDecelerateInterpolator());
////
////			// Start the animation
////			animator.start();
//		}
//	}
//	
//	void doShakeOnFreq() {
//		if (mainFrequencyEdit != null) {
//			mainFrequencyEdit.animate()
//				.translationX(-10f)
//				.setDuration(100)
//				.setInterpolator(new AccelerateInterpolator())
//				.start();
//
//			mainFrequencyEdit.animate()
//				.translationX(10f)
//				.setDuration(100)
//				.setInterpolator(new AccelerateInterpolator())
//				.start();
//
//			mainFrequencyEdit.animate()
//				.translationX(-10f)
//				.setDuration(100)
//				.setInterpolator(new AccelerateInterpolator())
//				.start();
//
//			mainFrequencyEdit.animate()
//				.translationX(10f)
//				.setDuration(100)
//				.setInterpolator(new AccelerateInterpolator())
//				.start();
//
//			mainFrequencyEdit.animate()
//				.translationX(0f)
//				.setDuration(230)
//				.setInterpolator(new AccelerateInterpolator())
//				.start();
//		}
//	}

	private void triggerVibration(long milliseconds) {
        Vibrator vibrator;

        // Initialize the Vibrator service based on the Android Version
        //if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.S) { // Android 12 (API 31) and above
        //    VibratorManager vibratorManager = (VibratorManager) getSystemService(Context.VIBRATOR_MANAGER_SERVICE);
        //    vibrator = vibratorManager.getDefaultVibrator();
        //} else { // Android 11 and below
        vibrator = (Vibrator) getSystemService(Context.VIBRATOR_SERVICE);
        //}

        // Safety check to ensure the device actually has a hardware vibrator
        if (vibrator != null && vibrator.hasVibrator()) {

            if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.O) { // Android 8.0 (API 26) and above
                // Creates a single, continuous buzz for the given duration at standard amplitude
                vibrator.vibrate(VibrationEffect.createOneShot(milliseconds, VibrationEffect.DEFAULT_AMPLITUDE));
            } else { // Deprecated legacy method for older Android versions
                vibrator.vibrate(milliseconds);
            }

        }
    }

	private void vibrateErrorPattern() {
//		runOnUiThread(new Runnable(){
//
//                @Override
//                public void run() {
//                    
//                    try {
//                        triggerVibration(450);
//                        Thread.sleep(230);
//                        triggerVibration(800);
//                    } catch (InterruptedException e) {
//                        e.printStackTrace();
//                        
//                    }
//                    
//                }
//            });

        triggerVibration(100);
        new Handler(Looper.getMainLooper()).postDelayed(new Runnable(){
                @Override
                public void run() {
                    triggerVibration(500);
                }
            }, 67);
	}

    private void doErrorAnimation(final View view) {
        // 1. Move right by 20 pixels
        view.animate()
            .translationX(20f)
            .setDuration(50)
            .withEndAction(new Runnable() {
                @Override
                public void run() {
                    // 2. Move left past center by 20 pixels
                    view.animate()
                        .translationX(-20f)
                        .setDuration(50)
                        .withEndAction(new Runnable() {
                            @Override
                            public void run() {
                                // 3. Move right past center by 15 pixels
                                view.animate()
                                    .translationX(15f)
                                    .setDuration(50)
                                    .withEndAction(new Runnable() {
                                        @Override
                                        public void run() {
                                            // 4. Return to the original center position
                                            view.animate()
                                                .translationX(0f)
                                                .setDuration(50);
                                        }
                                    });
                            }
                        });
                }
            });
    }

    private void refreshLog(ArrayList<String> str) {
        final ArrayAdapter<String> logStr = new ArrayAdapter<>(
            MainActivity.this, 
            android.R.layout.simple_list_item_1,
            str);


        mainListOutput.setAdapter(logStr);
        runOnUiThread(new Runnable(){
                @Override
                public void run() {
                    logStr.notifyDataSetChanged();
                }
            });
    }

    private void setLogDefaults() {
        if (!logList.isEmpty()) {
            logList.clear();
        }

        logList.add("Welcome to Jarred's Bytebeat Composer!");
        logList.add("This is your output log");
        logList.add("Where your application can play well or not...");
        logList.add("Alternatively, it can display errors here...");
        logList.add("Note: Comments are not allowed (e.g \"// hi\")");
        refreshLog(logList);
    }

    private long firstBackTime;

    @Override
    public void onBackPressed() {

        if (System.currentTimeMillis() - firstBackTime > 2000) {
            Toast.makeText(this, "Press again to exit", Toast.LENGTH_SHORT).show();
            firstBackTime = System.currentTimeMillis();
            return;
        }

        super.onBackPressed();
        //orientationEventListener.disable();
    }

//    @Override
//    protected void onStart() {
//        super.onStart();
//        orientationEventListener.enable();
//    }
//
//    @Override
//    protected void onResume() {
//        super.onResume();
//        orientationEventListener.enable();
//    }
//
//    @Override
//    protected void onStop() {
//        super.onStop();
//        orientationEventListener.disable();
//    }
//
//    @Override
//    protected void onPause() {
//        super.onPause();
//        orientationEventListener.disable();
//    }
    
    @Override
    protected void onDestroy() {

        if(player != null)
            player.stop();

        super.onDestroy();
    }

    @Override
    public boolean onCreateOptionsMenu(Menu menu) {
        getMenuInflater().inflate(R.menu.menu, menu);
        return super.onCreateOptionsMenu(menu);
    }

    @Override
    public boolean onOptionsItemSelected(MenuItem item) {
        int id = item.getItemId();

        switch (id) {
                // File
            case R.id.newMenu:
                AlertDialog comfirmDlg = new AlertDialog.Builder(this)
                    .setTitle("Are you sure?")
                    .setMessage("This will lose your work! You will need to save it!")
                    .setNeutralButton("Lose it", new DialogInterface.OnClickListener() {
                        @Override
                        public void onClick(DialogInterface dia, int which) {
                            if (mainFrequencyEdit != null) {
                                mainFrequencyEdit.setText("8000");
                            }
                            if (mainEditFormula != null) {
                                mainEditFormula.setText("t*(42&t>>10)");
                            }
                                                     
                            subtitle = "untitled";
                            getActionBar().setSubtitle(subtitle);
                        }
                    })
                    .setPositiveButton("Save it", new DialogInterface.OnClickListener() {
                        @Override
                        public void onClick(DialogInterface dia, int which) {
                            isNew = true;
                            startActivityForResult(saveFile, SAVEFILE_CODE);
                        }
                    })
                    .setNegativeButton("Cancel", null)
                    .create();
                comfirmDlg.show();
                break;

            case R.id.openMenu:
                startActivityForResult(openFile, OPENFILE_CODE);
                break;

            case R.id.saveMenu:
                startActivityForResult(saveFile, SAVEFILE_CODE);
                break;

            case R.id.exportMenu:
                final Dialog exportDlg = new Dialog(this);

                LayoutInflater inflater = getLayoutInflater();
                View v = inflater.inflate(R.layout.wav_export_dlg, null);

                final EditText editFormula = v.findViewById(R.id.wavexportdlgEditFormula);
                editFormula.setText(mainEditFormula.getText().toString());

                final EditText seconds = v.findViewById(R.id.wavexportdlgSeconds);               

                final Button cancelButton = v.findViewById(R.id.wavexportdlgButtonCancel);
                cancelButton.setOnClickListener(new View.OnClickListener() {

                        @Override
                        public void onClick(View view) {
                            exportDlg.dismiss();
                        }
                    });

                final Button exportButton = v.findViewById(R.id.wavexportdlgButtonExport);
                exportButton.setOnClickListener(new View.OnClickListener() {
                        @Override
                        public void onClick(View v) {
                            String secondsStr = seconds.getText().toString().trim();

                            if (secondsStr.isEmpty()) {
                                vibrateErrorPattern();
                                doErrorAnimation(seconds);
                                seconds.setError("You must enter seconds before you export!");
                            }

                            try {
                                secondsInt = Integer.parseInt(secondsStr);

                                exportDlg.dismiss();
                                startActivityForResult(exportFile, EXPORTFILE_CODE);
                            } catch (NumberFormatException ne) {
                                vibrateErrorPattern();
                                //ne.printStackTrace();
                                doErrorAnimation(seconds);
                                seconds.setError("Seconds must be numeric");
                            } catch (RuntimeException e) {
                                vibrateErrorPattern();
                                e.printStackTrace();
                                doErrorAnimation(seconds);
                                seconds.setError("java.lang.RuntimeException Error: " + e.toString());
                            }
                        }
                    });

                exportDlg.setCancelable(true);
                exportDlg.setContentView(v);
                exportDlg.show();

                break;

                //Log
            case R.id.clearLogMenu:
                AlertDialog clearComfirmDlg = new AlertDialog.Builder(this)
                    .setTitle("Are you sure?")
                    .setMessage("You will clear all logs!")
                    .setPositiveButton("Yes", new DialogInterface.OnClickListener() {

                        @Override
                        public void onClick(DialogInterface dia, int which) {
                            setLogDefaults();
                            Toast.makeText(getApplication(), "Log Cleared!", Toast.LENGTH_SHORT).show();
                        }
                    })
                    .setNegativeButton("No", null)
                    .create();
                clearComfirmDlg.show();
                break;


                // Application
            case R.id.aboutApp:
                final ImageView img = new ImageView(this);

                img.setWillNotCacheDrawing(true);
                img.setImageResource(R.drawable.version);

                AlertDialog aboutDialog = new AlertDialog.Builder(this)
                    .setTitle("About App")    
                    .setView(img)
                    .setPositiveButton("Ok", new DialogInterface.OnClickListener() {
                        @Override
                        public void onClick(DialogInterface dia, int which) {

                        }
                    })                   
                    .create();
                aboutDialog.show();
                return true;

            case R.id.exitApp:
                AlertDialog dialog = new AlertDialog.Builder(this)
                    .setTitle("Are you sure to quit?")
                    //.setMessage("")
                    .setPositiveButton("Ok", new DialogInterface.OnClickListener() {

                        @Override
                        public void onClick(DialogInterface dia, int which) {
                            finish();
                            System.exit(0);
                        }
                    })
                    .setNegativeButton("Cancel", null)
                    .create();
                dialog.show();
                return true;
        }
        return super.onOptionsItemSelected(item);
    }


    @Override
    protected void onActivityResult(int _requestCode, int _resultCode, Intent _data) {
        super.onActivityResult(_requestCode, _resultCode, _data);

        switch (_requestCode) {
            case EXPORTFILE_CODE:
                if (_resultCode == Activity.RESULT_OK) {
                    final Uri uri =
                        _data.getData();

                    exportDialog = new ProgressDialog(this);
                    exportDialog.setProgressStyle(
                        ProgressDialog.STYLE_SPINNER);
                    exportDialog.setTitle("Exporting");
                    exportDialog.setMessage("Please wait...");
                    exportDialog.setCancelable(false);
                    exportDialog.show();

                    new Thread(new Runnable(){

                            @Override
                            public void run() {
                                try {                                  
                                    String formula =
                                        mainEditFormula
                                        .getText()
                                        .toString();

                                    String frequency =
                                        mainFrequencyEdit
                                        .getText()
                                        .toString();

                                    int freqInt =
                                        Integer.parseInt(
                                        frequency);

                                    java.io.OutputStream out =
                                        getContentResolver()
                                        .openOutputStream(
                                        uri);

                                    if (typePos == 0) {
                                        WavExporter.export(
                                            formula,
                                            BytebeatType.BYTEBEAT_UNSIGNED,
                                            out,
                                            freqInt,
                                            secondsInt);
                                    } else if (typePos == 1) {
                                        WavExporter.export(
                                            formula,
                                            BytebeatType.BYTEBEAT_SIGNED,
                                            out,
                                            freqInt,
                                            secondsInt);
                                    }

                                    out.close();

                                    runOnUiThread(new Runnable(){

                                            @Override
                                            public void run() {
                                                exportDialog.dismiss();

                                                Toast.makeText(
                                                    MainActivity.this,
                                                    "Exported at: " + getFileName(uri),
                                                    Toast.LENGTH_LONG)
                                                    .show();
                                            }
                                        });



                                } catch (final Exception e) {

                                    runOnUiThread(new Runnable(){

                                            @Override
                                            public void run() {
                                                exportDialog.dismiss();

                                                Toast.makeText(
                                                    MainActivity.this,
                                                    "Export failed: "
                                                    + e.getMessage(),
                                                    Toast.LENGTH_LONG)
                                                    .show();

                                                e.printStackTrace();
                                            }
                                        });

                                }
                            }
                        }).start();
                }
                break;

            case SAVEFILE_CODE:
                if (_resultCode == Activity.RESULT_OK) {
                    Uri uri;

                    if (_data.getClipData() != null) {
                        uri = _data.getClipData().getItemAt(0).getUri();
                    } else {
                        uri = _data.getData();
                    }

                    if (uri != null) {
                        if (isNew) {
                            try {
                                writeTextToUri(uri, 
                                               mainEditFormula.getText().toString());
                            } catch (Exception e) {
                                e.printStackTrace();
                            }

                            if (mainFrequencyEdit != null) {
                                mainFrequencyEdit.setText("8000");
                            }
                            if (mainEditFormula != null) {
                                mainEditFormula.setText("t*(42&t>>10)");
                            }
                            
                            subtitle = "untitled";
                            getActionBar().setSubtitle(subtitle);
                            
                            isNew = false;
                        } else {
                            try {
                                writeTextToUri(uri, 
                                               mainEditFormula.getText().toString());
                            } catch (Exception e) {
                                e.printStackTrace();
                            }
                            
                            String filename = getFileName(uri);

                            if (filename != subtitle)
                                subtitle = filename;

                            getActionBar().setSubtitle(subtitle);
                        }
                    }
                } else {

                }
                break;

            case OPENFILE_CODE:
                if (_resultCode == Activity.RESULT_OK) {
                    Uri uri;

                    if (_data.getClipData() != null) {
                        uri = _data.getClipData().getItemAt(0).getUri();
                    } else {
                        uri = _data.getData();
                    }

                    if (uri != null) {                     
                        String filename = getFileName(uri);
                        
                        if (filename != subtitle)
                            subtitle = filename;
                            
                        getActionBar().setSubtitle(subtitle);

                        String fileText = readTextFromUri(uri);
                        mainEditFormula.setText(fileText);
                    }
                } else {

                }
                break;
            default:
                break;
		}
    }

    private String readTextFromUri(Uri uri) {
        StringBuilder stringBuilder = new StringBuilder();
        try {
            InputStream inputStream = getContentResolver().openInputStream(uri);
            BufferedReader reader = new BufferedReader(new InputStreamReader(inputStream));

            String line;
            while ((line = reader.readLine()) != null) {
                stringBuilder.append(line).append("\n");
            }
        } catch (IOException e) {
            vibrateErrorPattern();
            e.printStackTrace();
            doErrorAnimation(mainEditFormula);
            mainEditFormula.setError("Error reading file: " + e.getMessage());
        }
        return stringBuilder.toString();
    }


    private void writeTextToUri(Uri uri, String content) {
        try {
            ParcelFileDescriptor pfd = getContentResolver().openFileDescriptor(uri, "w");
            FileOutputStream fileOutputStream = new FileOutputStream(pfd.getFileDescriptor());
            OutputStreamWriter writer = new OutputStreamWriter(fileOutputStream);

            writer.write(content);
            writer.flush();
            Toast.makeText(getApplication(), "Saved to: " + getFileName(uri), Toast.LENGTH_SHORT).show();

        } catch (IOException e) {
            vibrateErrorPattern();
            e.printStackTrace();
            doErrorAnimation(mainEditFormula);
            mainEditFormula.setError("Error saving file: " + e.getMessage());
        }
    }

    private String getFileName(Uri uri) {
        String result = null;

        if ("content".equals(uri.getScheme())) {
            Cursor cursor = getContentResolver().query(
                uri,
                null,
                null,
                null,
                null
            );

            if (cursor != null) {
                try {
                    if (cursor.moveToFirst()) {
                        int nameIndex = cursor.getColumnIndex(
                            OpenableColumns.DISPLAY_NAME
                        );

                        if (nameIndex >= 0) {
                            result = cursor.getString(nameIndex);
                        }
                    }
                } finally {
                    cursor.close();
                }
            }
        }

        if (result == null) {
            result = uri.getLastPathSegment();
        }

        return result;
    }

    // Source - https://stackoverflow.com/a/9989900
    // Posted by cesards, modified by community. See post 'Timeline' for change history
    // Retrieved 2026-06-09, License - CC BY-SA 3.0
    /* private String getRealPathFromURI(Uri contentURI) {
     String result;
     Cursor cursor = getContentResolver().query(contentURI, null, null, null, null);
     if (cursor == null) { // Source is Dropbox or other similar local file path
     result = contentURI.getPath();
     } else { 
     cursor.moveToFirst(); 
     int idx = cursor.getColumnIndex(MediaStore.Images.ImageColumns.DATA); 
     result = cursor.getString(idx);
     cursor.close();
     }
     return result;
     } */
    // Unused due to cursor not initialized well on app
}
