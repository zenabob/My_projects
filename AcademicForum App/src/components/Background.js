import React from "react";
import { View, StyleSheet, SafeAreaView} from "react-native";
// import { SafeAreaView } from "react-native-safe-area-context";
// import { theme } from "../core/theme";

const Background = ({ children }) => {
  return (
    <SafeAreaView style={styles.backgroundContainer}>
      <View style={styles.backgroundContentContainer}>{children}</View>
    </SafeAreaView>
  );
};

const styles = StyleSheet.create({
  backgroundContainer: {
    flex: 1,
    // backgroundColor: theme.colors.background,
    justifyContent: "center",
    alignItems: "center",
  },
  backgroundContentContainer: {
    width: "100%",
    height: "100%",
    // maxWidth: 400,
    padding: 5,
    // backgroundColor: theme.colors.surface,
    // borderRadius: 8,
    justifyContent: "center",
    alignItems: "center",
  },
});

export default Background;