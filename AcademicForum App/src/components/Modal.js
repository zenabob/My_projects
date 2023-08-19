import React, {useEffect, useState} from 'react';
import {useTranslation} from 'react-i18next';
import {
    View,
    Modal,
    Text,
    TouchableOpacity,
    ScrollView,
    SafeAreaView,
    StyleSheet,
    Alert, ActivityIndicator,
} from 'react-native';
import {
    firstNameValidator,
    lastNameValidator,
    emailValidator,
    ageValidator,
    degreeValidator,
    subjectValidator,
    careerValidator,
    cityValidator,
    genderValidator,
    phoneValidator,
    imageUrlValidator,
    companyValidator,
} from '../Validators/Validator';
import {API_URL} from '@env';
import Dropdown from './DropDown';
import DatePicker from './DatePicker';
import TextInput from './TextInput';
import {CheckBox} from 'react-native-elements';
import {getDegreeList} from '../lists/degree';
import {getCityList} from '../lists/list';
import axios from 'axios';
import {UploadImage} from "./UploadImage";
import {DataToSelectOptions, dateFormate} from "../components/HelperFunction";

const MyModal = ({modalVisible, toggleModal, setIsConfirmed, setMessage, setErrorResponse}) => {
    const {t, i18n} = useTranslation();

    const [firstName, setFirstName] = useState({value: "", error: ""});
    const [lastName, setLastName] = useState({value: "", error: ""});
    const [email, setEmail] = useState({value: "", error: ""});
    const [selectedDate, setSelectedDate] = useState({value: "", error: ""});
    const [degree, setDegree] = useState({value: "", error: ""});
    const [subject, setSubject] = useState({value: "", error: ""});
    const [career, setCareer] = useState({value: "", error: ""});
    const [city, setCity] = useState({value: "", error: ""});
    const [gender, setGender] = useState({value: "", error: ""});
    const [phoneNumber, setPhoneNumber] = useState({value: "", error: ""});
    const [company, setCompany] = useState({value: "", error: ""});
    const [image, setImage] = useState({value: "", error: ""});
    const [checked, setChecked] = useState(false);
    const [loading, setLoading] = useState(false)

    const [addedCompany, setAddedCompany] = useState('');
    const [addedSubject, setAddedSubject] = useState('');
    const [addedCareer, setAddedCareer] = useState('');
    const [formatedDate, setFormatedDate] = useState('');

    const [subjects, setSubjects] = useState([]);
    const [companies, setCompanies] = useState([]);
    const [careers, setCareers] = useState([]);

    const subjectOptions = DataToSelectOptions(subjects, 'subject', 'subject');
    const careerOptions = DataToSelectOptions(careers, 'career', 'career');
    const companyOptions = DataToSelectOptions(companies, 'company', 'company');

    useEffect(() => {
        fetchAcademics();
    }, []);

    useEffect(() => {
        if (addedCareer !== '') {
            setCareer({value: addedCareer, error: ''});
        }
    }, [addedCareer]);

    useEffect(() => {
        if (addedCompany) {
            setCompany({value: addedCompany, error: ''});
        }
    }, [addedCompany]);

    useEffect(() => {
        if (addedSubject) {
            setSubject({value: addedSubject, error: ''});
        }
    }, [addedSubject]);

    useEffect(() => {
        if (selectedDate.value) {
            setFormatedDate(dateFormate(selectedDate.value));
        } else {
            setFormatedDate('');
        }
    }, [selectedDate.value]);

    const fetchAcademics = async () => {
        try {
            const subjects = await axios.get(`${API_URL}/academic/subjects`);
            const careers = await axios.get(`${API_URL}/academic/careers`);
            const companies = await axios.get(`${API_URL}/academic/companies`);
            setSubjects(subjects.data);
            setCareers(careers.data);
            setCompanies(companies.data);
        } catch (error) {
            console.error(error);
        }
    };

    const handleSend = async () => {
        setLoading(true);
        const fnameError = firstNameValidator(firstName.value);
        const lnameError = lastNameValidator(lastName.value);
        const emailError = emailValidator(email.value);
        const aError = ageValidator(formatedDate);
        const degreeError = degreeValidator(degree.value);
        const careerError = careerValidator(career.value);
        const companyError = companyValidator(company.value);
        const subjectError = subjectValidator(subject.value);
        const cityError = cityValidator(city.value);
        const phoneError = phoneValidator(phoneNumber.value);
        const imageError = imageUrlValidator(image.value);
        const genderError = genderValidator(gender.value);

        if (fnameError || lnameError || emailError || aError || degreeError || careerError ||
            companyError || subjectError || cityError || phoneError || imageError || genderError) {
            setFirstName({...firstName, error: fnameError});
            setLastName({...lastName, error: lnameError});
            setEmail({...email, error: emailError});
            setSelectedDate({...selectedDate, error: aError});
            setDegree({...degree, error: degreeError});
            setCareer({...career, error: careerError});
            setCompany({...company, error: companyError});
            setSubject({...subject, error: subjectError});
            setCity({...city, error: cityError});
            setPhoneNumber({...phoneNumber, error: phoneError});
            setGender({...gender, error: genderError});
            setImage({...image, error: imageError});
            setChecked(false);
            setLoading(false);
            return;
        }
        try {
            // await new Promise(resolve => setTimeout(resolve, 10000));
            const response = await axios.post(`${API_URL}/academic`, {
                firstName: firstName.value,
                lastName: lastName.value,
                email: email.value,
                imageUrl: image.value,
                age: formatedDate,
                degree: degree.value,
                subject: subject.value,
                career: career.value,
                city: city.value,
                gender: gender.value,
                phone: phoneNumber.value,
                company: company.value,
                isAgree: checked,
            });

            console.log(response.data);
            if (response.data.status === 201) {
                setMessage(t('academicpage.dialog.success'));
                toggleModal();
                setIsConfirmed(true);
                // Clear form fields after successful submission
                setFirstName('');
                setLastName('');
                setSelectedDate('');
                setEmail('');
                setCity('');
                setDegree('');
                setSubject('');
                setCompany('');
                setCareer('');
                setGender({value: '', error: ''});
                setPhoneNumber('');
                setImage('');
                fetchAcademics();
            } else if (response.data.status === 409) {
                setMessage(t('academicpage.dialog.error.email'));
                setErrorResponse(true);
                toggleModal();
                setIsConfirmed(true);
            } else {
                setMessage(t('academicpage.dialog.error'));
                setErrorResponse(true);
                toggleModal();
                setIsConfirmed(true);
            }
            setChecked(false);
            setLoading(false);
        } catch (error) {
            if (error.response) {
                // The request was made and the server responded with a status code
                console.log('Status:', error.response.status);
                console.log('Data:', error.response.data);
            } else if (error.request) {
                // The request was made but no response was received
                console.log('Request:', error.request);
            } else {
                // Something happened in setting up the request that triggered an Error
                console.log('Error:', error.message);
            }
            console.log('Config:', error.config);
            // Alert.alert('Error', 'An error occurred while sending the form. Please try again.');
            setChecked(false);
            setLoading(false);
        }
    }

    const getTextAlignment = () => {
        const {i18n} = useTranslation();
        if (i18n.language === 'AR' || i18n.language === 'HE') {
            return 'right'; // Right-to-left alignment for Spanish
        } else {
            return 'left'; // Left-to-right alignment for other languages
        }
    };

    const textAlignment = getTextAlignment();


    return (
        <Modal visible={modalVisible} animationType="slide" onRequestClose={toggleModal}>
            <SafeAreaView style={styles.modalContainer}>
                <ScrollView>
                    <TouchableOpacity onPress={toggleModal} style={styles.closeButton}>
                        <Text style={styles.closeButtonText}>{t('academicpage.dialog.close')}</Text>
                    </TouchableOpacity>
                    <Text style={styles.GuideText}>{t('academicpage.dialog.sub.title')}</Text>
                    <TextInput label={t('academicpage.dialog.first.name')}
                               returnKeyType="next"
                               value={firstName.value}
                               onChangeText={(text) => setFirstName({value: text, error: ""})}
                               error={!!firstName.error}
                               errorText={firstName.error}
                    />
                    <TextInput label={t('academicpage.dialog.last.name')}
                               returnKeyType="next"
                               value={lastName.value}
                               onChangeText={(text) => setLastName({value: text, error: ""})}
                               error={!!lastName.error}
                               errorText={lastName.error}/>
                    <DatePicker
                        placeholder={t('academicpage.dialog.Birhdate')}
                        label={t('academicpage.dialog.Birhdate')}
                        value={selectedDate.value}
                        onChange={(date) => setSelectedDate({value: date, error: ''})}
                        error={selectedDate.error}
                    />
                    <TextInput label={t('academicpage.dialog.email')}
                               returnKeyType="next"
                               value={email.value}
                               onChangeText={(text) => setEmail({value: text, error: ""})}
                               error={!!email.error}
                               errorText={email.error}/>
                    <TouchableOpacity style={styles.dropDownContainer}>
                        <Dropdown placeholder={t('academicpage.dialog.city')} label={t('academicpage.dialog.city')}
                                  data={cities.map((cityIn) => ({
                                      label: cityIn.label,
                                      value: cityIn.label,
                                  }))}
                                  value={city.value}
                                  setValue={setCity}
                                  setAddedValue={null}
                                  errorText={city.error}/>
                        <Dropdown placeholder={t('academicpage.dialog.degree')} label={t('academicpage.dialog.degree')}
                                  data={degrees.map((degreeIn) => ({
                                      label: degreeIn.label,
                                      value: degreeIn.id.toString(),
                                  }))}
                                  value={degree.value}
                                  setAddedValue={null}
                                  setValue={setDegree}
                                  errorText={degree.error}/>
                        <Dropdown placeholder={t('academicpage.dialog.subject')}
                                  label={t('academicpage.dialog.subject')}
                                  data={subjectOptions}
                                  value={addedSubject || subject.value}
                                  setAddedValue={setAddedSubject}
                                  setValue={setSubject}
                                  errorText={subject.error}/>
                        <Dropdown placeholder={t('academicpage.dialog.company')}
                                  label={t('academicpage.dialog.company')}
                                  data={companyOptions}
                                  value={addedCompany || company.value}
                                  setAddedValue={setAddedCompany}
                                  setValue={setCompany}
                                  errorText={company.error}/>
                        <Dropdown placeholder={t('academicpage.dialog.job')} label={t('academicpage.dialog.job')}
                                  data={careerOptions}
                                  value={addedCareer || career.value}
                                  setAddedValue={setAddedCareer}
                                  setValue={setCareer}
                                  errorText={career.error}/>
                        <Dropdown placeholder={t('academicpage.dialog.sex')} label={t('academicpage.dialog.sex')}
                                  data={genderData}
                                  value={gender.value}
                                  setValue={setGender}
                                  setAddedValue={null}
                                  errorText={gender.error}/>
                    </TouchableOpacity>
                    <TextInput label={t('academicpage.dialog.phone')}
                               returnKeyType="next"
                               value={phoneNumber.value}
                               onChangeText={(text) => setPhoneNumber({value: text, error: ""})}
                               error={!!phoneNumber.error}
                               errorText={phoneNumber.error}/>
                    <UploadImage
                        setImage={(val) => setImage({value: val, error: ''})}
                        error={image.error}
                    />
                    <CheckBox
                        title={t('academicpage.dialog.checkbox')}
                        checked={checked}
                        onPress={() => {
                            setChecked(!checked)
                        }}
                    />
                    <TouchableOpacity style={checked ? styles.sendButton : styles.sendButtonCheck} onPress={handleSend}
                                      disabled={!checked}>
                        <Text style={styles.sendButtonText}>{t('contactpage.submit')}</Text>
                    </TouchableOpacity>
                    {loading ? (
                        <View style={styles.loadingContainer}>
                            <ActivityIndicator size="small" color="#0000ff"/>
                        </View>
                    ) : null}
                    <View style={{height: 150}}></View>
                </ScrollView>
            </SafeAreaView>
        </Modal>

    );
};

export default MyModal;

const styles = StyleSheet.create({
    modalContainer: {
        flex: 1,
        padding: 16,
        justifyContent: "center",
    },
    closeButton: {
        alignSelf: 'center',
        backgroundColor: 'white',
        padding: 10,
        borderRadius: 5,
        elevation: 2,
    },
    closeButtonText: {
        color: 'blue',
        fontSize: 16,
    },
    GuideText: {
        alignSelf: 'center',
        paddingHorizontal: 12
    },
    input: {
        width: '80%',
        height: 50,
        borderColor: 'gray',
        borderWidth: 0.5,
        borderRadius: 8,
        paddingHorizontal: 8,
    },
    dateInput: {
        borderWidth: 0,
        borderBottomWidth: 0,
        alignItems: 'flex-start',
    },
    dateText: {
        paddingTop: 10,
    },
    sendButton: {
        backgroundColor: "green",
        borderRadius: 8,
        paddingVertical: 12,
        paddingHorizontal: 16,
        width: '50%',
        alignSelf: 'center', // Add this line to center the button
        marginTop: 20, // Add margin-top to create some space between the checkbox and the button
        marginBottom: 10,
        alignItems: 'center',
    },
    sendButtonCheck: {
        backgroundColor: 'lightgreen',
        borderRadius: 8,
        paddingVertical: 12,
        paddingHorizontal: 16,
        width: '50%',
        alignSelf: 'center',
        marginTop: 20,
        alignItems: 'center',
    },
    sendButtonText: {
        fontSize: 16,
        fontWeight: "bold",
        color: "white",
    },
    loadingContainer: {
        alignItems: 'center',
    },
});

const cities = getCityList;
const degrees = getDegreeList;

const genderData = [
    {label: 'Male', value: '1'},
    {label: 'Female', value: '2'},
];
