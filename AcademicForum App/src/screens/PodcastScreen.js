import React, {useEffect, useRef, useState} from 'react';
import {
    SafeAreaView,
    StyleSheet,
    View,
    ScrollView,
    Text,
    Image,
    TouchableOpacity,
    I18nManager,
    Dimensions,
    Platform,
} from 'react-native';
import { API_URL } from '@env';
import { StatusBar } from 'expo-status-bar';
import { useTranslation } from 'react-i18next';
import { Video } from 'expo-av';
import axios from 'axios';
import { FontAwesome } from '@expo/vector-icons';
import End from '../components/End';

export default function PodcastScreen({navigation}) {
    const [videos, setVideos] = useState([]);
    const {t, i18n} = useTranslation();
    const isRTL = I18nManager.isRTL;

    // SideBar & Languages
    const [isSideBarOpen, setSideBarOpen] = useState(false);
    const sidebarRef = useRef(null);

    const handleMenuPress = () => {
        setSideBarOpen(!isSideBarOpen);
    }

    const handleLanguageChange = (language) => {
        i18n.changeLanguage(language);
        setSideBarOpen(false);
    };

    useEffect(() => {
        fetchVideos();
    }, []);

    const fetchVideos = async () => {
        try {
          const response = await axios.get(`${API_URL}/podcasts/isActive/true`);
          setVideos(response.data);
        } catch (error) {
          console.error(error);
        }
    };

    const renderItem = ({ item }) => (
        <View style={styles.videoContainer} key={item.id}>
          <Video
            source={{ uri: item.podcastUrl }}
            style={styles.video}
            // shouldPlay
            resizeMode="cover"
            useNativeControls 
          />
          <Text style={styles.videoTitle}>{item.title}</Text>
        </View>
    );


    return (
        <SafeAreaView style={styles.container}>
            <StatusBar style="dark" />
            <View style={styles.sideBarContainer}>
                <TouchableOpacity onPress={handleMenuPress} style={styles.menuButton}>
                    {isSideBarOpen ? (
                        <FontAwesome name="times" size={24} color="black"/>
                    ) : (
                        <FontAwesome name="bars" size={24} color="black"/>
                    )}
                </TouchableOpacity>
                {isSideBarOpen && (
                    <ScrollView
                        ref={sidebarRef}
                        contentContainerStyle={styles.sidebarContentContainer}
                    >
                        <View style={styles.languageContainer}>
                            <TouchableOpacity
                                onPress={() => handleLanguageChange('EN')}
                                style={styles.sidebarButton}
                            >
                                <Text style={styles.sidebarButtonText}>EN</Text>
                            </TouchableOpacity>
                            <TouchableOpacity
                                onPress={() => handleLanguageChange('AR')}
                                style={styles.sidebarButton}
                            >
                                <Text style={styles.sidebarButtonText}>AR</Text>
                            </TouchableOpacity>
                            <TouchableOpacity
                                onPress={() => handleLanguageChange('HE')}
                                style={styles.sidebarButton}
                            >
                                <Text style={styles.sidebarButtonText}>HE</Text>
                            </TouchableOpacity>
                        </View>
                        <TouchableOpacity
                            onPress={() => {
                                handleMenuPress();
                                navigation.navigate('SearchScreen');
                            }}
                            style={styles.sidebarButton}
                        >
                            <Text style={styles.sidebarButtonText}>{t('navbar.academic')}</Text>
                        </TouchableOpacity>
                        <TouchableOpacity
                            onPress={() => {
                                handleMenuPress();
                                navigation.navigate('PodcastScreen');
                            }}
                            style={styles.sidebarButton}
                        >
                            <Text style={styles.sidebarButtonText}>{t('navbar.podcast')}</Text>
                        </TouchableOpacity>
                        <TouchableOpacity
                            onPress={() => {
                                handleMenuPress();
                                navigation.navigate('ContactUsScreen');
                            }}
                            style={styles.sidebarButton}
                        >
                            <Text style={styles.sidebarButtonText}>{t('navbar.contact')}</Text>
                        </TouchableOpacity>
                    </ScrollView>
                )}
            </View>
            <ScrollView  nestedScrollEnabled
              contentContainerStyle={styles.pageContainer}
            //   ref={scrollViewRef}
              showsVerticalScrollIndicator={false}
            >
               <TouchableOpacity onPress={()=> navigation.navigate("StartScreen")} style={styles.logoContainer}>
              <Image
                        source={require("../../assets/FinalLogo.png")}
                        style={styles.logo}
                    />
                </TouchableOpacity>
                <TouchableOpacity onPress={()=> navigation.navigate("StartScreen")}>
                
          
                </TouchableOpacity>
                <View style={styles.pageContent}>
                    <Text style={[styles.firstText, isRTL && styles.rtlText]}>{t('podcastpage.desc')}</Text>
                    <Text style={[styles.secondText, isRTL && styles.rtlText]}>{t('podcastpage.subdesc')}</Text>
                    {videos.map((item) => renderItem({ item }))}
                </View>
                <End style={styles.End} navigation={navigation} /> 
                    
            </ScrollView>
        </SafeAreaView>
    );

}

const styles = StyleSheet.create({
    container: {
        flex: 1,
        backgroundColor:'white',
        ...Platform.select({
            android: {
                paddingTop: 40,
            },
        }),
    },
    languageContainer: {
        flexDirection: 'row',
        justifyContent: 'space-between',
    },
    sideBarContainer: {
        flexGrow: 1,
    },
    sidebarContentContainer: {
        flexGrow: 1,
        paddingLeft: 64,
        paddingRight: 64,
        width: '100%',
        height: '100%',
    },
    menuButton: {
        paddingLeft: 16,
        paddingTop: 8,
        height: 40,
    },
    sidebarButton: {
        marginTop: 8,
        marginBottom: 16,
        paddingVertical: 8,
        paddingHorizontal: 16,
        backgroundColor: '#f58723',
        borderRadius: 8,
    },
    sidebarButtonText: {
        alignSelf: 'center',
        fontSize: 16,
        fontWeight: 'bold',
        color: 'white',
    },
    logoContainer: {
        width: 200,
        height: 75,
        alignSelf: 'center', 
        resizeMode: "contain",
    },
    logo: {
        width: 200,
        height: 75,
        resizeMode: "contain",
        alignSelf: 'center',
    },
    pageContainer: {
        flexGrow: 1,
        alignItems: "center",
        paddingBottom: 20,
    },
    pageContent: {
        flex: 1,
        alignItems: 'center',
        justifyContent: 'center',
        padding: 16,
    },
    firstText: {
        color: "black",
        textAlign: 'center',
        fontSize:17,
        fontWeight: "bold",
    },
    secondText: {
        color: "black",
        paddingTop: 10,
        textAlign: 'center',
        fontSize:17,
        fontWeight: "bold",
    },
    videoContainer: {
        paddingTop: 30,
        alignItems: "center",
        width: '100%',
        marginBottom: 16,
    },
    video: {
      width: 365,
      height: 200,
    },
    videoTitle: {
      fontSize: 16,
      fontWeight: 'bold',
      marginTop: 8,
    },
    rtlText: {
        textAlign: 'right',
    },
});